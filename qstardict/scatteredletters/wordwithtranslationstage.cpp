/*****************************************************************************
 * wordwithtranslationstage.cpp - QStarDict, a free dictionary application   *
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

#include "wordwithtranslationstage.h"

#include "speaker.h"

namespace QStarDict
{

WordWithTranslationStage::WordWithTranslationStage(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
    m_speaker = new Speaker();

    transcriptionLabel->setVisible(false);

    connect(nextWordButton, SIGNAL(clicked()), SLOT(nextWord()));
    connect(nextStageButton, SIGNAL(clicked()), SLOT(onNextStage()));
}

void WordWithTranslationStage::setWords(const QVector<WordForTraining> &wordsList)
{
    m_wordsList = wordsList;
    m_currentWordIndex = 0;
}

void WordWithTranslationStage::startStage()
{
    nextWordButton->setVisible(true);
    nextStageButton->setVisible(false);

    m_currentWordIndex = 0;
    showCurrentWord();
    speak();
}

void WordWithTranslationStage::nextWord()
{
    m_currentWordIndex++;

    if (m_currentWordIndex == m_wordsList.size())
        emit nextStage();
    else
    {
        showCurrentWord();
        speak();
    }
}

void WordWithTranslationStage::onNextStage()
{
    emit nextStage();
}

void WordWithTranslationStage::speak()
{
    if (m_wordsList.size() == 0)
        return;

    m_speaker->speak(m_wordsList[m_currentWordIndex].word());;
}

void WordWithTranslationStage::showCurrentWord()
{
    wordLabel->setText(m_wordsList[m_currentWordIndex].word());
    translationLabel->setText(m_wordsList[m_currentWordIndex].translation());

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

}

