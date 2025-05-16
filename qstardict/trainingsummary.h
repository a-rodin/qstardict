/*****************************************************************************
 * trainingsummary.h - QStarDict, a dictionary application for learning      *
 *                     languages                                             *
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

#ifndef TRAININGSUMMARY_H
#define TRAININGSUMMARY_H

#include "ui_trainingsummary.h"

namespace QStarDict
{

class TrainingSummary: public QWidget, private Ui::TrainingSummary
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        TrainingSummary(QWidget *parent = nullptr);

        /**
         * Set the number of studed words.
         */
        void setStudiedWords(unsigned n);

        /**
         * Set the number of words for repetition.
         */
        void setWordsForRepetition(unsigned n);

        /**
         * Update the displayed progress.
         */
        void setProgress(unsigned studiedToday, unsigned plannedToday);

    signals:
        void continueTraining();
        void endTraining();

    protected:
        void keyPressEvent(QKeyEvent *event);

    private slots:
        void on_configureButton_clicked();
        void on_continueTrainingButton_clicked();
        void on_endTrainingButton_clicked();
};

}

#endif // TRAININGSUMMARY_H

