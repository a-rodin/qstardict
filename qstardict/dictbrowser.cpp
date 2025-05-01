/*****************************************************************************
 * dictbrowser.cpp - QStarDict, a StarDict clone written using Qt            *
 * Copyright (C) 2007-2025 Alexander Rodin                                   *
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

#include "dictbrowser.h"

#include <QDesktopServices>
#include <QMouseEvent>
#include <QTextBlock>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include "../plugins/dictplugin.h"
#include "application.h"
#include "ipa.h"
#include "speaker.h"

namespace
{
const QString translationCSS = 
    "body {\n"
        "font-size: 10pt; }\n"
    "font.dict_name {\n"
        "color: blue;\n"
        "font-style: italic; }\n"
    "font.title {\n"
        "font-size: 16pt;\n"
        "font-weight: bold; }\n"
    "font.explanation {\n"
        "color: #7f7f7f;\n"
        "font-style: italic; }\n"
    "font.abbreviature {\n"
        "font-style: italic; }\n"
    "font.example {\n"
        "font-style: italic; }\n"
    "font.transcription {\n"
        "font-weight: bold; }\n";
}

namespace QStarDict
{

DictBrowser::DictBrowser(QWidget *parent)
    : QTextBrowser(parent),
      m_dict(0),
      m_highlighted(false),
      m_searchUndo(false),
      m_showIpaPronouncers(true)
{
    document()->setDefaultStyleSheet(translationCSS);
    setOpenLinks(false);
    setOpenExternalLinks(false);
    connect(this, SIGNAL(anchorClicked(const QUrl &)), SLOT(on_anchorClicked(const QUrl &)));
    connect(this, &QTextBrowser::sourceChanged, this, &DictBrowser::on_sourceChanged);
}

QVariant DictBrowser::loadResource(int type, const QUrl &name)
{
    if (type == QTextDocument::HtmlResource && name.scheme() == "qstardict")
    {
        QString str = name.toString(QUrl::RemoveScheme);
        QString result = m_dict->translate(str);
        if (result.isEmpty())
            result = "<table><tr><td><img src=\":/pics/dialog-warning.png\" width=64 height=64/></td><td valign=middle>" +
                tr("The word <b>%1</b> is not found.").arg(str) +
                "</td></tr></table>";
        return "<title>Translation for \"" + str + "\"</title>\n"
            + "<body>" + result + "</body>";
    }
    else if (name.scheme() == "plugin")
    {
        DictPlugin *plugin = m_dict->plugin(name.host());
        if (! plugin)
            return QVariant();
        return plugin->resource(type, name);
    }
    return QTextBrowser::loadResource(type, name);
}

void DictBrowser::search(const QString & exp, QTextDocument::FindFlags options)
{
    bool found = false;

    QList<QTextEdit::ExtraSelection> extraSelections;

    moveCursor(QTextCursor::Start);
    QColor color = QColor(Qt::gray).lighter(130);

    while (find(exp, options))
    {
        found = true;

        QTextEdit::ExtraSelection extra;
        extra.format.setBackground(color);

        extra.cursor = textCursor();
        extraSelections.append(extra);
    }

    setExtraSelections(extraSelections);

    emit searchResult(found);
}

void DictBrowser::mouseMoveEvent(QMouseEvent *event)
{
    if (m_highlighted)
    {
        m_oldCursor.setCharFormat(m_oldFormat);
        m_highlighted = false;
    }
    if (event->modifiers().testFlag(Qt::ControlModifier))
    {
        QTextCursor cursor = cursorForPosition(event->pos());
        cursor.select(QTextCursor::WordUnderCursor);
        QString selection = cursor.selection().toPlainText().simplified();
        if (m_dict->isTranslatable(selection))
        {
            m_oldCursor = cursor;
            m_oldFormat = cursor.charFormat();

            QTextCharFormat format = m_oldFormat;
            format.setForeground(Qt::blue);
            format.setFontUnderline(true);
            cursor.setCharFormat(format);

            m_highlighted = true;
        }
    }

    QTextBrowser::mouseMoveEvent(event);
}

void DictBrowser::mousePressEvent(QMouseEvent *event)
{
    if (event->modifiers().testFlag(Qt::ControlModifier))
    {
        QTextCursor cursor = cursorForPosition(event->pos());
        cursor.select(QTextCursor::WordUnderCursor);
        QString selection = cursor.selection().toPlainText().simplified();
        if (m_dict->isTranslatable(selection))
        {
            setSource(selection);
            if (m_highlighted)
                m_highlighted = false;
        }
    }
    QTextBrowser::mousePressEvent(event);
}

void DictBrowser::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_showIpaPronouncers)
    {
        // check whether the click was on a pronounce button
        QTextCursor cursor = cursorForPosition(event->pos());
        cursor.select(QTextCursor::WordUnderCursor);
        QUrl url = cursor.charFormat().anchorHref();
        if (url.scheme() == "pronounce")
        {
            QString ipa = QUrl::fromPercentEncoding(url.toString(QUrl::RemoveScheme).toUtf8());
            QString kirshenbaum = Ipa::ipaToKirshenbaum(ipa);
            Application::instance()->espeakSpeaker()->speak("[[" + kirshenbaum + "]]");
            return;
        }
    }

    QTextBrowser::mousePressEvent(event);
}

void DictBrowser::on_anchorClicked(const QUrl &link)
{
    QString scheme = link.scheme();
    if (scheme == "plugin" || scheme == "qrc")
        setSource(link);
    else
        QDesktopServices::openUrl(link);
}

void DictBrowser::on_sourceChanged(const QUrl &)
{
    if (m_showIpaPronouncers)
        addIpaPronouncers();
}

void DictBrowser::addIpaPronouncers()
{
    const static QVector<QRegularExpression> transcriptionRegExps = {
        Ipa::broadTranscriptionRegExp(),
        Ipa::narrowTranscriptionRegExp()
    };
    for (const QRegularExpression &transcriptionRegExp: transcriptionRegExps)
    {
        QTextDocument *doc = document();
        QTextCursor cursor;
        int position = 0;
        while (! (cursor = doc->find(transcriptionRegExp, position)).isNull())
        {
            QString transcription = cursor.selectedText();
            QString ipa = transcriptionRegExp.match(transcription).captured(2);

            cursor.insertHtml("<font class=\"transcription\">" + transcription + "</font>"
                "<a href=\"pronounce:" + QUrl::toPercentEncoding(ipa) + "\">"
                "<img style=\"vertical-align: middle\" src=\":/pics/pronounce.png\">"
                "</a>");
            position = cursor.position();
        }
    }
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

