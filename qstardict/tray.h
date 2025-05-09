/*****************************************************************************
 * tray.h - QStarDict, a dictionary application for learning languages       *
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

#ifndef TRAY_H
#define TRAY_H

#include <QSystemTrayIcon>

namespace QStarDict
{

class Tray: public QSystemTrayIcon
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        Tray(QObject *parent = nullptr);

        /**
         * Destructor.
         */
        virtual ~Tray();

        /**
         * Save settings, such as the visibility of the tray item.
         */
        void saveSettings();

    private slots:
        void on_activated(QSystemTrayIcon::ActivationReason reason);
        void on_actionSettings_triggered();
        void setScanEnabled(bool enabled);

    private:
        void loadSettings();
};

}

#endif // TRAY_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

