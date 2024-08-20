/*****************************************************************************
 * typeinstage.cpp - QStarDict, a free dictionary application                *
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

#include "typeinstage.h"

#include <QKeyEvent>
#include "speaker.h"

namespace QStarDict
{

TypeInStage::TypeInStage(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
    m_speaker = new Speaker();
    connect(speakButton, SIGNAL(clicked()), this, SLOT(speak()));
    connect(wordEdit, SIGNAL(returnPressed()), this, SLOT(onEnterPressed()));
    connect(enterButton, SIGNAL(clicked()), this, SLOT(onWordEntered()));
    connect(nextWordButton, SIGNAL(clicked()), this, SLOT(nextWord()));
    connect(nextStageButton, SIGNAL(clicked()), this, SLOT(onNextStage()));

    enterButton->setVisible(true);
    nextWordButton->setVisible(false);
    nextStageButton->setVisible(false);
}

TypeInStage::~TypeInStage()
{
    delete m_speaker;
}

void TypeInStage::setWords(const QVector<WordForTraining> &wordsList)
{
    m_wordsList = wordsList;
    m_currentWordIndex = 0;
    m_wordsWithErrors.clear();
}

void TypeInStage::speak()
{
    if (m_wordsList.size() == 0)
        return;

    m_speaker->speak(m_wordsList[m_currentWordIndex].word());;
}

void TypeInStage::onEnterPressed()
{
    if (enterButton->isVisible())
        enterButton->click();
    else if (nextWordButton->isVisible())
        nextWordButton->click();
    else if (nextStageButton->isVisible())
        nextStageButton->click();
}

void TypeInStage::startStage()
{
    correctLabel->setText("");
    wordEdit->setText("");
    enterButton->setVisible(true);
    nextWordButton->setVisible(false);
    nextStageButton->setVisible(false);

    wordEdit->setFocus();
    m_currentWordIndex = 0;
    m_wordsWithErrors.clear();
    speak();
}

void TypeInStage::onWordEntered()
{
    correctLabel->setText(tr("<p align=\"center\">%1</p>").arg(m_wordsList[m_currentWordIndex].word()));

    if (wordEdit->text() != m_wordsList[m_currentWordIndex].word())
        m_wordsWithErrors.push_back(m_wordsList[m_currentWordIndex]);

    enterButton->setVisible(false);
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

void TypeInStage::nextWord()
{
    m_currentWordIndex++;

    correctLabel->setText("");
    wordEdit->setText("");
    enterButton->setVisible(true);
    nextWordButton->setVisible(false);
    nextStageButton->setVisible(false);
    wordEdit->setFocus();

    speak();
}

void TypeInStage::onNextStage()
{
    emit nextStage();
}

}
