/*****************************************************************************
 * scatteredletters.h - QStarDict, a free dictionary application             *
 * Copyright (C) 2024 Alexander Rodin                                        *
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

#ifndef SCATTEREDLETTERS_H
#define SCATTEREDLETTERS_H

#include <QVector>
#include <QWidget>

class QHBoxLayout;
class QPushButton;
class QVBoxLayout;

namespace QStarDict
{

class ScatteredLetters: public QWidget
{
    Q_OBJECT

    public:
        ScatteredLetters(QWidget *parent = nullptr);
        virtual ~ScatteredLetters();

        void setWord(const QString &word);

    public slots:
        void surrender();

    signals:
        void completed(bool errors);
        void error(int errorsCount);

        void enterPressed();
	
    protected:
        void keyReleaseEvent(QKeyEvent *event);
        void keyPressEvent(QKeyEvent *event);

    private:
        QString m_word;
        int m_errorsCount;
        bool m_completed;
        QVector<QPushButton*> m_letterButtons;
        QVector<QPushButton*> m_typedInLetterButtons;
        QHBoxLayout *m_topLayout;
        QHBoxLayout *m_bottomLayout;
        QVBoxLayout *m_layout;

        void deleteLayoutsAndButtons();
};

}

#endif // SCATTEREDLETTERS_H
