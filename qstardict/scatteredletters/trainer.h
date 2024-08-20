/*****************************************************************************
 * trainer.h - QStarDict, a free dictionary application                      *
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

#ifndef TRAINER_H
#define TRAINER_H

#include <QSet>
#include <QVector>
#include <QWidget>
#include "wordfortraining.h"

class QLabel;

namespace QStarDict
{

class ScatteredLettersStage;
class TypeInStage;

class Trainer: public QWidget
{
    Q_OBJECT

    public:
        Trainer(QWidget *parent = 0);
        virtual ~Trainer();

        void setWords(const QVector<WordForTraining> &wordsList);
        void start();

    private slots:
        void scatteredLettersStage();
        void scatteredLettersStageFinished();
        void typeInStage();
        void typeInStageFinished();
	void allStagesFinished();

    private:
        QVector<WordForTraining> m_wordsList;

        QVector<WordForTraining> m_scatteredLettersWordsList;
        QVector<WordForTraining> m_typeInWordsList;

	QSet<WordForTraining> m_wordsWithErrorsList;

        ScatteredLettersStage *m_scatteredLettersStage;
        TypeInStage *m_typeInStage;
	QLabel *m_allStagesFinishedLabel;
};

};

#endif // TRAINER_H
