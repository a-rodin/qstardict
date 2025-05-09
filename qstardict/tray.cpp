/*****************************************************************************
 * tray.cpp - QStarDict, a dictionary application for learning languages     *
 * Copyright (C) 2008-2025 Alexander Rodin                                   *
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

#include "tray.h"

#include <QClipboard>
#include <QMenu>
#include <QSettings>

#include "application.h"
#include "mainwindow.h"
#include "popupwindow.h"
#include "settingsdialog.h"

namespace QStarDict
{

Tray::Tray(QObject *parent)
    : QSystemTrayIcon(parent)
{
    QMenu *trayMenu = new QMenu(tr("QStarDict"));

    QAction *actionScan = new QAction(QIcon(":/pics/edit-select.png"), tr("&Scan"), this);
    actionScan->setCheckable(true);
    actionScan->setChecked(Application::instance()->popupWindow()->isScan());
    setScanEnabled(Application::instance()->popupWindow()->isScan());
    connect(actionScan, &QAction::toggled,
            Application::instance()->popupWindow(), &PopupWindow::setScan);
    connect(Application::instance()->popupWindow(), &PopupWindow::scanChanged,
            actionScan, &QAction::setChecked);
    connect(Application::instance()->popupWindow(), &PopupWindow::scanChanged,
            this, &Tray::setScanEnabled);
    trayMenu->addAction(actionScan);

    QAction *actionSettings = new QAction(QIcon(":/pics/configure.png"), tr("&Configure QStarDict"), this);
    connect(actionSettings, &QAction::triggered,
        this, &Tray::on_actionSettings_triggered);
    trayMenu->addAction(actionSettings);

    QAction *actionQuit = new QAction(QIcon(":/pics/application-exit.png"), tr("&Quit"), this);
    connect(actionQuit, &QAction::triggered,
        Application::instance(), &Application::saveSettingsAndQuit);
    trayMenu->addAction(actionQuit);

    setContextMenu(trayMenu);
    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            SLOT(on_activated(QSystemTrayIcon::ActivationReason)));

    loadSettings();
}

Tray::~Tray()
{
    saveSettings();
}

void Tray::on_activated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
        // It's quite uncomfortable on OS X to handle show/hide main window
        // in all cases... at least for me (petr)
#ifndef Q_WS_MAC
            Application::instance()->mainWindow()->setVisible(!
                    Application::instance()->mainWindow()->isVisible());
#else
            Application::instance()->mainWindow()->show();
#endif
            break;
        case QSystemTrayIcon::MiddleClick:
            Application::instance()->popupWindow()->showTranslation(Application::clipboard()->text(QClipboard::Selection));
            break;
        default:
            ; // nothing
    }
}

void Tray::on_actionSettings_triggered()
{
    SettingsDialog dialog(Application::instance()->mainWindow());
    dialog.exec();
}

void Tray::setScanEnabled(bool enabled)
{
    QIcon icon(enabled ? ":/pics/qstardict.png" : ":/pics/qstardict-disabled.png");
    setIcon(icon);
    setToolTip(tr("QStarDict: scanning is %1").arg(enabled ? tr("enabled") : tr("disabled")));
}

void Tray::saveSettings()
{
    QSettings config;
    config.setValue("TrayIcon/visible", isVisible());
}

void Tray::loadSettings()
{
    QSettings config;
    setVisible(config.value("TrayIcon/visible", true).toBool());
}

}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

