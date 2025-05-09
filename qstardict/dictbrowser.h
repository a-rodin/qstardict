/*****************************************************************************
 * dictbrowser.h - QStarDict, a dictionary application for learning          *
 *                 languages                                                 *
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

#ifndef DICTBROWSER_H
#define DICTBROWSER_H

#include <QTextBrowser>

#include <QTextCursor>
#include <QTextCharFormat>
#include "dictcore.h"

namespace QStarDict
{

class Toast;

/**
 * The DictBrowser widget provides view of translations from given dictionary.
 */
class DictBrowser: public QTextBrowser
{
    Q_OBJECT
    public:
        /**
         * Construct an empty DictWidget.
         */
        DictBrowser(QWidget *parent = nullptr);

        /**
         * Set source dictionary.
         * Warning: DictBrowser will copy only a pointer to dict. So set dictionaries
         * allocated from heap and don't destroy it befor DictWidget.
         */
        void setDict(DictCore *dict)
        { m_dict = dict; }
        /**
         * Return the pointer to the dictionary.
         */
        const DictCore* dict() const
        { return m_dict; }

        /**
          * Set whether to show buttons near IPA transcriptions which allow
          * the user to listen the pronunciation of these transcriptions.
          */
        void setShowIpaPronouncers(bool showIpaPronouncers)
        { m_showIpaPronouncers = showIpaPronouncers; }

        /**
          * Return true if the IPA pronouncers are enabled.
          */
        bool showIpaPronouncers() const
        { return m_showIpaPronouncers; }

        /**
         * A reimplementation of QTextBrowser's "loadResource" method.
         */
        QVariant loadResource(int type, const QUrl &name);

        /**
         * Return the original source HTML of the loaded article. The result
         * is different from the result of "toHtml()" method.
         */
        const QString &loadedArticleHtml()
        { return m_loadedArticleHtml; }

        /**
         * Show a toast message inside of the dictionary browser.
         */
        void showToast(const QString &html);

    signals:
        void searchResult(bool success);

    public slots:
        void search(const QString & exp, QTextDocument::FindFlags options);

    protected:
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);

    private slots:
        void on_anchorClicked(const QUrl &link);
        void on_sourceChanged(const QUrl &link);
        void on_toastTimer_timeout();
        void on_toastCloseTimer_timeout();

    private:
        DictCore *m_dict;
        QString m_loadedArticleHtml;

        QTextCursor m_oldCursor;
        QTextCharFormat m_oldFormat;
        bool m_highlighted;
        bool m_searchUndo;

        bool m_showIpaPronouncers;

        Toast *m_toast;
        QTimer *m_toastTimer;
        QTimer *m_toastCloseTimer;

        void addIpaPronouncers();
};

}

#endif // DICTBROWSER_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

