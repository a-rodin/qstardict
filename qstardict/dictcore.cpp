/*****************************************************************************
 * dictcore.cpp - QStarDict, a StarDict clone written with using Qt          *
 * Copyright (C) 2008 Alexander Rodin                                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with this program; if not, write to the Free Software Foundation, Inc.,   *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.               *
 *****************************************************************************/

#include "dictcore.h"

#include <QFileInfoList>
#include <QFileInfo>
#include <QDir>
#include <QRegExp>
#include <QSettings>
#include <QDebug>
#include <QPluginLoader>
#include "dictplugin.h"

namespace QStarDict
{

DictCore::DictCore(QObject *parent)
    : QObject(parent)
{
    loadSettings();
}

DictCore::~DictCore()
{
    saveSettings();
    foreach (QPluginLoader *loader, m_plugins)
        delete loader;
}

bool DictCore::isTranslatable(const QString &word)
{
    for (QList<Dictionary>::const_iterator i = m_loadedDicts.begin(); i != m_loadedDicts.end(); ++i) 
    {
        if (! m_plugins.contains(i->plugin()))
            continue;
        if (qobject_cast<DictPlugin*>(m_plugins[i->plugin()]->instance())->isTranslatable(i->name(), word))
            return true;
    }
    return false;
}

QString DictCore::translate(const QString &word)
{
    QString result;
    for (QList<Dictionary>::const_iterator i = m_loadedDicts.begin(); i != m_loadedDicts.end(); ++i)
    {
        if (! m_plugins.contains(i->plugin()))
            continue;
        DictPlugin *plugin = qobject_cast<DictPlugin*>(m_plugins[i->plugin()]->instance());
        if (! plugin->isTranslatable(i->name(), word))
            continue;
        DictPlugin::Translation translation = plugin->translate(i->name(), word);
        result += "<p><font class=\"normal\">\n"
            "<font class=\"dict_name\">" + translation.dictName() + "</font><br>\n"
            "<font class=\"title\">" + translation.title() + "</font><br>\n"
            + translation.translation() + "</font></p>\n";
    }
    return result;
}

QStringList DictCore::findSimilarWords(const QString &word)
{
    QStringList result;
    for (QList<Dictionary>::const_iterator i = m_loadedDicts.begin(); i != m_loadedDicts.end(); ++i)
    {
        if (! m_plugins.contains(i->plugin()))
            continue;
        DictPlugin *plugin = qobject_cast<DictPlugin*>(m_plugins[i->plugin()]->instance());
        if (! plugin->features().testFlag(DictPlugin::SearchSimilar))
            continue;
        result += plugin->findSimilarWords(i->name(), word);
    }
    return result;
}

QStringList DictCore::avialablePlugins() const
{
    QStringList result;
#ifdef Q_OS_UNIX
    QFileInfoList files = QDir(QSTARDICT_PLUGINS_DIR).entryInfoList(QStringList("lib*.so"),
                  QDir::Files | QDir::NoDotAndDotDot);
    for (QFileInfoList::const_iterator i = files.begin(); i != files.end(); ++i)
        result << i->fileName().mid(3, i->fileName().length() - 6);
#elif defined Q_OS_WIN
    // TODO
#else
#error "Function DictCore::avialablePlugins() is not implemented on this platform"
#endif
    return result;
}

void DictCore::setLoadedPlugins(const QStringList &loadedPlugins)
{
    for (QHash <QString, QPluginLoader*>::iterator i = m_plugins.begin(); i != m_plugins.end(); ++i)
        delete *i;
    m_plugins.clear();

    for (QStringList::const_iterator i = loadedPlugins.begin(); i != loadedPlugins.end(); ++i)
    {
#ifdef Q_OS_UNIX
        QString pluginFilename = QSTARDICT_PLUGINS_DIR "/" "lib" + *i + ".so";
#elif defined Q_OS_WIN
        // TODO
        QString pluginFilename = QSTARDICT_PLUGINS_DIR "/" "lib" + *i + ".dll";
#else
#error "Function DictCore::setLoadedPlugins(const QStringList &loadedPlugins) is not avialable on this platform"
#endif
        QPluginLoader *plugin = new QPluginLoader(pluginFilename);
        if (! plugin->load())
        {
            qWarning() << plugin->errorString();
            delete plugin;
            continue;
        }
        m_plugins[*i] = plugin;
    }
}

QList<DictCore::Dictionary> DictCore::avialableDicts() const
{
    QList<Dictionary> result;
    for (QHash<QString, QPluginLoader*>::const_iterator i = m_plugins.begin(); i != m_plugins.end(); ++i)
    {
        DictPlugin *plugin = qobject_cast<DictPlugin*>((*i)->instance());
        QStringList dicts = plugin->avialableDicts();
        for (QStringList::const_iterator j = dicts.begin(); j != dicts.end(); ++j)
            result << Dictionary(i.key(), *j);
    }
    return result;
}

void DictCore::setLoadedDicts(const QList<Dictionary> &loadedDicts)
{
    QHash<QString, QStringList> dicts;
    for (QList<Dictionary>::const_iterator i = loadedDicts.begin(); i != loadedDicts.end(); ++i)
        dicts[i->plugin()] << i->name();
    for (QHash<QString, QStringList>::const_iterator i = dicts.begin(); i != dicts.end(); ++i)
    {
        if (! m_plugins.contains(i.key()))
            continue;
        DictPlugin *plugin = qobject_cast<DictPlugin*>(m_plugins[i.key()]->instance());
        plugin->setLoadedDicts(*i);
        dicts[i.key()] = plugin->loadedDicts();
    }
    m_loadedDicts.clear();
    for (QList<Dictionary>::const_iterator i = loadedDicts.begin(); i != loadedDicts.end(); ++i)
        if (dicts.contains(i->plugin()) && dicts[i->plugin()].contains(i->name()))
            m_loadedDicts << *i;
}

void DictCore::saveSettings()
{
    QSettings config;
    config.setValue("DictCore/loadedPlugins", loadedPlugins());
    QStringList rawDictsList;
    for (QList<Dictionary>::const_iterator i = m_loadedDicts.begin(); i != m_loadedDicts.end(); ++i)
        rawDictsList << i->plugin() << i->name();
    config.setValue("DictCore/loadedDicts", rawDictsList);
}

void DictCore::loadSettings()
{
    QSettings config;
    setLoadedPlugins(config.value("DictCore/loadedPlugins", avialablePlugins()).toStringList());
    QStringList rawDictsList = config.value("DictCore/loadedDicts").toStringList();
    if (rawDictsList.isEmpty())
        setLoadedDicts(avialableDicts());
    else
    {
        QList<Dictionary> dicts;
        for (QStringList::const_iterator i = rawDictsList.begin(); i != rawDictsList.end(); i += 2)
            dicts << Dictionary(*i, *(i + 1));
        setLoadedDicts(dicts);
    }
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

