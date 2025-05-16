/*****************************************************************************
 * trainer.h - QStarDict, a dictionary application for learning languages    *
 * Copyright (C) 2024-2025 Alexander Rodin                                   *
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

#ifndef TRAINER_H
#define TRAINER_H

#include <QDialog>
#include <QSet>
#include <QVector>
#include "wordfortraining.h"

class QLabel;

namespace QStarDict
{

class ChooseTranslationStage;
class ScatteredLettersStage;
class TrainingSummary;
class TypeInStage;
class Vocabulary;
class WordWithTranslationStage;

class Trainer: public QDialog
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        Trainer(QWidget *parent = nullptr);

        /**
         * Destructor.
         */
        virtual ~Trainer();

        /**
         * Set a vocabulary instance.
         */
        void setVocabulary(Vocabulary *vocabulary)
        { m_vocabulary = vocabulary; }

        /**
         * Start training.
         */
        void start();

        /**
         * Return the number of words per round.
         */
        unsigned wordsPerRound()
        { return m_wordsPerRound; }

        /**
         * Set the number of words per round.
         */
        void setWordsPerRound(unsigned wordsPerRound)
        { m_wordsPerRound = wordsPerRound; }

        /**
         * Return the number of words planned for studying per day.
         */
        unsigned wordsPerDay()
        { return m_wordsPerDay; }

        /**
         * Set the number of words planned for studying per day.
         */
        void setWordsPerDay(unsigned wordsPerDay);

        /**
         * Save the settings of the trainer.
         */
        void saveSettings();

    private slots:
        void wordWithTranslationStage();
        void wordWithTranslationStageFinished();
        void chooseTranslationStage();
        void chooseTranslationStageFinished();
        void scatteredLettersStage();
        void scatteredLettersStageFinished();
        void typeInStage();
        void typeInStageFinished();
        void allStagesFinished();

    private:
        Vocabulary *m_vocabulary;
        unsigned m_wordsPerRound;
        unsigned m_wordsPerDay;

        QVector<WordForTraining> m_wordsList;

        QVector<WordForTraining> m_wordWithTranslationWordsList;
        QVector<WordForTraining> m_chooseTranslationWordsList;
        QVector<WordForTraining> m_scatteredLettersWordsList;
        QVector<WordForTraining> m_typeInWordsList;

        QSet<WordForTraining> m_wordsWithErrorsList;

        WordWithTranslationStage *m_wordWithTranslationStage;
        ChooseTranslationStage *m_chooseTranslationStage;
        ScatteredLettersStage *m_scatteredLettersStage;
        TypeInStage *m_typeInStage;
        TrainingSummary *m_trainingSummary;

        void setWords(const QVector<WordForTraining> &wordsList);
        void removeWidgets();
        void loadSettings();
};

};

#endif // TRAINER_H
