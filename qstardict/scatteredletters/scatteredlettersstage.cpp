/*****************************************************************************
 * scatteredlettersstage.cpp - QStarDict, a free dictionary application      *
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

#include "scatteredlettersstage.h"

#include <QKeyEvent>

namespace QStarDict
{

ScatteredLettersStage::ScatteredLettersStage(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    connect(scatteredLetters, SIGNAL(error(int)), this, SLOT(updateErrors(int)));
    connect(scatteredLetters, SIGNAL(completed(bool)), this, SLOT(onWordCompleted(bool)));
    connect(scatteredLetters, SIGNAL(enterPressed()), this, SLOT(onEnterPressed()));
    connect(surrenderButton, SIGNAL(clicked()), scatteredLetters, SLOT(surrender()));
    connect(nextWordButton, SIGNAL(clicked()), this, SLOT(nextWord()));
    connect(nextStageButton, SIGNAL(clicked()), this, SLOT(onNextStage()));
}

void ScatteredLettersStage::startStage()
{
    surrenderButton->setVisible(true);
    nextWordButton->setVisible(false);
    nextStageButton->setVisible(false);

    m_currentWordIndex = 0;
    setWord(m_wordsList[m_currentWordIndex]);
}

void ScatteredLettersStage::updateErrors(int errorsCount)
{
    QString color;
    if (errorsCount <= 1)
        color = "#0000ff";
    else
        color = "#ff0000";
    errorsLabel->setText(tr("<p align=\"right\"><font color=\"%1\"><b>Errors: %2</b></font></p>")
            .arg(color)
            .arg(errorsCount));
}

void ScatteredLettersStage::onWordCompleted(bool errors)
{
    if (errors)
        m_wordsWithErrors.push_back(m_wordsList[m_currentWordIndex]);

    surrenderButton->setVisible(false);
    if (m_currentWordIndex == m_wordsList.size() - 1)
    {
        nextWordButton->setVisible(false);
        nextStageButton->setVisible(true);
    }
    else
    {
        nextWordButton->setVisible(true);
        nextStageButton->setVisible(false);
    }
}

void ScatteredLettersStage::setWord(const WordForTraining &wordForTraining)
{
    translationLabel->setText(tr("<p align=\"center\">%1</p>").arg(wordForTraining.translation()));
    updateErrors(0);
    scatteredLetters->setWord(wordForTraining.word());
    scatteredLetters->setFocus();

    surrenderButton->setVisible(true);
    nextWordButton->setVisible(false);
    nextStageButton->setVisible(false);
}

void ScatteredLettersStage::setWords(const QVector<WordForTraining> &wordsList)
{
    m_wordsList = wordsList;
    m_currentWordIndex = 0;
    m_wordsWithErrors.clear();
}

void ScatteredLettersStage::onEnterPressed()
{
    if (nextWordButton->isVisible())
        nextWordButton->click();
    else if (nextStageButton->isVisible())
        nextStageButton->click();
}

void ScatteredLettersStage::nextWord()
{
    m_currentWordIndex++;
    setWord(m_wordsList[m_currentWordIndex]);
}

void ScatteredLettersStage::onNextStage()
{
    emit nextStage();
}

}
