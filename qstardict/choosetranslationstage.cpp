/*****************************************************************************
 * choosetranslationstage.cpp - QStarDict, a dictionary application for      *
 *                              learning languages                           *
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

#include "choosetranslationstage.h"
#include <cstdlib>

#include <QKeyEvent>

namespace QStarDict
{

ChooseTranslationStage::ChooseTranslationStage(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
    
    m_translationButtons
            << translation1Button
            << translation2Button
            << translation3Button
            << translation4Button;

    connect(nextWordButton, SIGNAL(clicked()), this, SLOT(nextWord()));
    connect(nextStageButton, SIGNAL(clicked()), this, SLOT(onNextStage()));

    for (QPushButton *translationButton: m_translationButtons)
    {
        connect(translationButton, &QPushButton::released, [=]() {
            onTranslationSelected(translationButton->text());
        });
    }

    nextWordButton->setVisible(false);
    nextStageButton->setVisible(false);
    correctLabel->setVisible(false);
    wrongLabel->setVisible(false);
}

ChooseTranslationStage::~ChooseTranslationStage()
{
}

void ChooseTranslationStage::setWords(const QVector<WordForTraining> &wordsList)
{
    m_wordsList = wordsList;
    m_currentWordIndex = 0;
    m_wordsWithErrors.clear();
}

int ChooseTranslationStage::proposedTranslationsCount() const
{
    return m_wordsList.size() * (TRANSLATIONS_PER_WORD - 1);
}

void ChooseTranslationStage::setProposedTranslations(const QStringList &translations)
{
    m_proposedTranslations.clear();
    int translationsCounter = 0;
    for (int i = 0; i < m_wordsList.size(); i++)
    {
        QStringList translationsForWord;
        int correctTranslationPosition = random() % TRANSLATIONS_PER_WORD;
        if (translations.size() > 0)
            for (int j = 0; j < correctTranslationPosition; j++)
            {
                translationsForWord << translations[translationsCounter % translations.size()];
                translationsCounter++;
            }
        translationsForWord << m_wordsList[i].translation();
        if (translations.size() > 0)
            for (int j = correctTranslationPosition + 1; j < TRANSLATIONS_PER_WORD; j++)
            {
                translationsForWord << translations[translationsCounter % translations.size()];
                translationsCounter++;
            }
        m_proposedTranslations[m_wordsList[i]] = translationsForWord;
    }
}

void ChooseTranslationStage::onEnterPressed()
{
    if (nextWordButton->isVisible())
        nextWordButton->click();
    else if (nextStageButton->isVisible())
        nextStageButton->click();
}

void ChooseTranslationStage::startStage()
{
    nextWordButton->setVisible(false);
    nextStageButton->setVisible(false);

    m_currentWordIndex = 0;
    m_wordsWithErrors.clear();
    displayWord();
}

void ChooseTranslationStage::onNextStage()
{
    emit nextStage();
}

void ChooseTranslationStage::nextWord()
{
    m_currentWordIndex++;

    nextWordButton->setVisible(false);
    nextStageButton->setVisible(false);
    displayWord();
}

void ChooseTranslationStage::onTranslationSelected(const QString &translation)
{
    if (translation == m_wordsList[m_currentWordIndex].translation())
        correctLabel->setVisible(true);
    else
    {
        wrongLabel->setVisible(true);
        m_wordsWithErrors.push_back(m_wordsList[m_currentWordIndex]);
    }

    for (int i = 0; i < m_translationButtons.size(); i++)
    {
        if (m_translationButtons[i]->text() == m_wordsList[m_currentWordIndex].translation())
        {
            QFont font = m_translationButtons[i]->font();
            font.setBold(true);
            m_translationButtons[i]->setFont(font);
        }
        m_translationButtons[i]->setEnabled(false);
    }

    if (m_currentWordIndex == m_wordsList.size() - 1)
    {
        nextWordButton->setVisible(false);
        nextStageButton->setVisible(true);
        nextStageButton->setFocus();
    }
    else
    {
        nextWordButton->setVisible(true);
        nextWordButton->setFocus();
        nextStageButton->setVisible(false);
    }
}

void ChooseTranslationStage::displayWord()
{
    wordLabel->setText(m_wordsList[m_currentWordIndex].word());
    const QStringList &proposedTranslations = m_proposedTranslations[m_wordsList[m_currentWordIndex]];

    int i;
    for (i = 0; i < proposedTranslations.size(); i++)
    {
        m_translationButtons[i]->setText(proposedTranslations[i]);
        m_translationButtons[i]->setChecked(false);
        QFont font = m_translationButtons[i]->font();
        font.setBold(false);
        m_translationButtons[i]->setFont(font);
        m_translationButtons[i]->setEnabled(true);
        m_translationButtons[i]->setVisible(true);
    }
    for (; i < m_translationButtons.size(); i++)
    {
        m_translationButtons[i]->setVisible(false);
    }

    correctLabel->setVisible(false);
    wrongLabel->setVisible(false);
    this->setFocus();
}

}
