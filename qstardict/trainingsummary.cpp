/*****************************************************************************
 * trainingsummary.cpp - QStarDict, a dictionary application for learning    *
 *                       languages                                           *
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

#include "trainingsummary.h"

#include <QKeyEvent>

#include "application.h"
#include "mainwindow.h"
#include "settingsdialog.h"
#include "vocabulary.h"

namespace QStarDict
{

TrainingSummary::TrainingSummary(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
}

void TrainingSummary::setStudiedWords(unsigned n)
{
    if (n == 1)
        studiedLabel->setText(tr("<font color=\"#2FAA23\"><b>Studied:</b></font> 1 word"));
    else
        studiedLabel->setText(tr("<font color=\"#2FAA23\"><b>Studied:</b></font> %1 words").arg(n));
}

void TrainingSummary::setWordsForRepetition(unsigned n)
{
    if (n == 1)
        forRepetitionLabel->setText(tr("<font color=\"#FF0000\"><b>For repetition:</b></font> 1 word"));
    else
        forRepetitionLabel->setText(tr("<font color=\"#FF0000\"><b>For repetition:</b></font> %1 words").arg(n));
}

void TrainingSummary::setProgress(unsigned studiedToday, unsigned plannedToday)
{
    if (studiedToday == 1)
        progressLabel->setText(tr("Progress today: 1/%1 word").arg(plannedToday));
    else
        progressLabel->setText(tr("Progress today: %1/%2 words").arg(studiedToday).arg(plannedToday));
    progressBar->setValue(studiedToday);
    progressBar->setMaximum(plannedToday);
}

void TrainingSummary::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Return:
            continueTrainingButton->click();
            break;
        case Qt::Key_Escape:
            endTrainingButton->click();
            break;
        default:
            QWidget::keyPressEvent(event);
            break;
    }
}

void TrainingSummary::on_configureButton_clicked()
{
    SettingsDialog dialog(Application::instance()->mainWindow(), SettingsDialog::Tab::Training);
    dialog.exec();
}

void TrainingSummary::on_continueTrainingButton_clicked()
{
    emit continueTraining();
}

void TrainingSummary::on_endTrainingButton_clicked()
{
    emit endTraining();
}

}
