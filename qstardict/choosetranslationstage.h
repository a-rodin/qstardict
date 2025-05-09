/*****************************************************************************
 * choosetranslationstage.h - QStarDict, a dictionary application for        *
 *                            learning languages                             *
 * Copyright (C) 2025 Alexander Rodin                                        *
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

#ifndef CHOOSETRANSLATIONSTAGE_H
#define CHOOSETRANSLATIONSTAGE_H

#include "ui_choosetranslationstage.h"
#include "wordfortraining.h"

namespace QStarDict
{

class IpaSpeaker;

class ChooseTranslationStage: public QWidget, private Ui::ChooseTranslationStage
{
    Q_OBJECT

    public:
        ChooseTranslationStage(QWidget *parent = nullptr);
        virtual ~ChooseTranslationStage();

        void setWords(const QVector<WordForTraining> &wordsList);

        int proposedTranslationsCount() const;
        void setProposedTranslations(const QStringList &translations);

        QVector<WordForTraining> wordsWithErrors()
        { return m_wordsWithErrors; }

    signals:
        void nextStage();

    public slots:
        void startStage();

    private slots:
        void onEnterPressed();
        void onNextStage();
        void nextWord();
        void onTranslationSelected(const QString &translation);

    private:
        const int TRANSLATIONS_PER_WORD = 4;

        QVector<WordForTraining> m_wordsList;
        QHash<WordForTraining, QStringList> m_proposedTranslations;
        int m_currentWordIndex;
        QVector<WordForTraining> m_wordsWithErrors;

        QVector<QPushButton*> m_translationButtons;

        void displayWord();
};

}

#endif // CHOOSETRANSLATIONSTAGE_H

