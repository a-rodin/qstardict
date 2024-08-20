/*****************************************************************************
 * scatteredlettersstage.h - QStarDict, a free dictionary application        *
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

#ifndef SCATTEREDLETTERSSTAGE_H
#define SCATTEREDLETTERSSTAGE_H

#include "ui_scatteredlettersstage.h"

#include "wordfortraining.h"

namespace QStarDict
{

class ScatteredLettersStage: public QWidget, private Ui::ScatteredLettersStage
{
    Q_OBJECT

    public:
        ScatteredLettersStage(QWidget *parent = 0);

        void setWords(const QVector<WordForTraining> &wordsList);
        void startStage();

        QVector<WordForTraining> wordsWithErrors()
        { return m_wordsWithErrors; }

    signals:
        void nextStage();

    private slots:
        void updateErrors(int errorsCount);
        void onWordCompleted(bool errors);
        void onEnterPressed();
        void nextWord();
        void onNextStage();

    private:
        QVector<WordForTraining> m_wordsList;
        int m_currentWordIndex;
        QVector<WordForTraining> m_wordsWithErrors;

        void setWord(const WordForTraining &wordForTraining);
};

}

#endif // SCATTEREDLETTERSSTAGE_H
