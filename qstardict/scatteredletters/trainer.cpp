/*****************************************************************************
 * trainer.cpp - QStarDict, a free dictionary application                    *
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

#include "trainer.h"

#include <QVBoxLayout>

#include "scatteredlettersstage.h"
#include "typeinstage.h"
#include "wordwithtranslationstage.h"

namespace QStarDict
{

Trainer::Trainer(QWidget *parent)
    : QWidget(parent)
{
    m_scatteredLettersStage = new ScatteredLettersStage;
    m_typeInStage = new TypeInStage;
    m_allStagesFinishedLabel = new QLabel;

    // connect(m_wordWithTranslationStage, SIGNAL(nextStage()), this, SLOT(wordWithTranslationStageFinished()));
    connect(m_scatteredLettersStage, SIGNAL(nextStage()), this, SLOT(scatteredLettersStageFinished()));
    connect(m_typeInStage, SIGNAL(nextStage()), this, SLOT(typeInStageFinished()));

    new QVBoxLayout(this);
}

Trainer::~Trainer()
{
    delete m_scatteredLettersStage;
    delete m_typeInStage;
    delete m_allStagesFinishedLabel;
}

void Trainer::setWords(const QVector<WordForTraining> &wordsList)
{
    m_wordsList = wordsList;
    m_wordWithTranslationWordsList = wordsList;
    m_scatteredLettersWordsList = wordsList;
    m_typeInWordsList = wordsList;
}

void Trainer::start()
{
    wordWithTranslationStage();
}

void Trainer::wordWithTranslationStage()
{
    // TODO
}

void Trainer::wordWithTranslationStageFinished()
{
    // TODO
}

void Trainer::scatteredLettersStage()
{
    m_scatteredLettersStage->setWords(m_scatteredLettersWordsList);

    removeWidgets();
    layout()->addWidget(m_scatteredLettersStage);
    m_scatteredLettersStage->setVisible(true);
    m_scatteredLettersStage->startStage();
}

void Trainer::scatteredLettersStageFinished()
{
    m_scatteredLettersWordsList = m_scatteredLettersStage->wordsWithErrors();
    if (! m_typeInWordsList.isEmpty())
        typeInStage();
}

void Trainer::typeInStage()
{
    m_typeInStage->setWords(m_wordsList);

    removeWidgets();
    layout()->addWidget(m_typeInStage);
    m_typeInStage->setVisible(true);
    m_typeInStage->startStage();
}

void Trainer::typeInStageFinished()
{
    m_typeInWordsList = m_typeInStage->wordsWithErrors();
    if (! m_scatteredLettersWordsList.isEmpty())
        scatteredLettersStage();
}

void Trainer::allStagesFinished()
{
    layout()->removeWidget(m_scatteredLettersStage);
    layout()->removeWidget(m_typeInStage);
    m_allStagesFinishedLabel->setText(tr("Studied: %1 words, for repetition: %2 words")
            .arg(m_wordsList.size() - m_wordsWithErrorsList.size())
            .arg(m_wordsWithErrorsList.size()));
    layout()->addWidget(m_allStagesFinishedLabel);
}

void Trainer::removeWidgets()
{
    layout()->removeWidget(m_wordWithTranslationStage);
    layout()->removeWidget(m_typeInStage);
    layout()->removeWidget(m_scatteredLettersStage);
    m_wordWithTranslationStage->setVisible(false);
    m_typeInStage->setVisible(false);
    m_scatteredLettersStage->setVisible(false);
}

}
