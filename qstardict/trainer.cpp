/*****************************************************************************
 * trainer.cpp - QStarDict, a dictionary application for learning languages  *
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

#include "trainer.h"

#include <QMessageBox>
#include <QSettings>
#include <QVBoxLayout>

#include "application.h"
#include "choosetranslationstage.h"
#include "mainwindow.h"
#include "scatteredlettersstage.h"
#include "trainingsummary.h"
#include "typeinstage.h"
#include "vocabulary.h"
#include "wordwithtranslationstage.h"

namespace QStarDict
{

Trainer::Trainer(QWidget *parent)
    : QDialog(parent),
    m_vocabulary(nullptr)
{
    setWindowTitle(tr("QStarDict Training"));

    m_wordWithTranslationStage = new WordWithTranslationStage;
    m_chooseTranslationStage = new ChooseTranslationStage;
    m_scatteredLettersStage = new ScatteredLettersStage;
    m_typeInStage = new TypeInStage;
    m_trainingSummary = new TrainingSummary;

    connect(m_wordWithTranslationStage, &WordWithTranslationStage::nextStage,
            this, &Trainer::wordWithTranslationStageFinished);
    connect(m_chooseTranslationStage, &ChooseTranslationStage::nextStage,
            this, &Trainer::chooseTranslationStageFinished);
    connect(m_scatteredLettersStage, &ScatteredLettersStage::nextStage,
            this, &Trainer::scatteredLettersStageFinished);
    connect(m_typeInStage, &TypeInStage::nextStage,
            this, &Trainer::typeInStageFinished);
    connect(m_trainingSummary, &TrainingSummary::continueTraining,
            this, &Trainer::start);
    connect(m_trainingSummary, &TrainingSummary::endTraining,
            this, &QDialog::reject);

    QLayout *layout = new QHBoxLayout(this);
    setLayout(layout);
    setMinimumSize(640, 300);

    loadSettings();
}

Trainer::~Trainer()
{
    delete m_wordWithTranslationStage;
    delete m_chooseTranslationStage;
    delete m_scatteredLettersStage;
    delete m_typeInStage;
    delete m_trainingSummary;
}

void Trainer::start()
{
    setWords(m_vocabulary->getWordsForTraining(m_wordsPerRound));
    if (m_wordsList.size() == 0)
    {
        hide();
        QMessageBox::warning(Application::instance()->mainWindow(), tr("QStarDict Training"),
                tr("There are no words for training. Please add words for training using the "
                "<img width=\"24\" height=\"24\" src=\":/pics/word-add.png\"> button in translations before training."));
    }
    else
    {
        wordWithTranslationStage();
        show();
    }
}

void Trainer::setWordsPerDay(unsigned wordsPerDay)
{
    m_wordsPerDay = wordsPerDay;
    if (m_trainingSummary->isVisible())
    {
        m_trainingSummary->setProgress(
                m_vocabulary->numberOfWordsStudiedToday(),
                m_wordsPerDay);
    }
}

void Trainer::saveSettings()
{
    QSettings config;
    config.setValue("Trainer/wordsPerRound", m_wordsPerRound);
    config.setValue("Trainer/wordsPerDay", m_wordsPerDay);
}

void Trainer::wordWithTranslationStage()
{
    m_wordWithTranslationStage->setWords(m_wordWithTranslationWordsList);

    removeWidgets();
    layout()->addWidget(m_wordWithTranslationStage);
    m_wordWithTranslationStage->setVisible(true);
    m_wordWithTranslationStage->startStage();
}

void Trainer::wordWithTranslationStageFinished()
{
    chooseTranslationStage();
}

void Trainer::chooseTranslationStage()
{
    m_chooseTranslationStage->setWords(m_chooseTranslationWordsList);

    unsigned count = m_chooseTranslationStage->proposedTranslationsCount();
    QStringList skipTranslations;
    for (const auto &word: m_chooseTranslationWordsList)
        skipTranslations << word.translation();
    QStringList translations = m_vocabulary->getRandomTranslations(count, skipTranslations);
    m_chooseTranslationStage->setProposedTranslations(translations);

    removeWidgets();
    layout()->addWidget(m_chooseTranslationStage);
    m_chooseTranslationStage->setVisible(true);
    m_chooseTranslationStage->startStage();
}

void Trainer::chooseTranslationStageFinished()
{
    for (const WordForTraining &wordWithError: m_chooseTranslationStage->wordsWithErrors())
        m_wordsWithErrorsList.insert(wordWithError);
    m_chooseTranslationWordsList = m_chooseTranslationStage->wordsWithErrors();
    if (! m_scatteredLettersWordsList.isEmpty())
        scatteredLettersStage();
    else if (! m_typeInWordsList.isEmpty())
        typeInStage();
    else if (! m_chooseTranslationWordsList.isEmpty())
        chooseTranslationStage();
    else
        allStagesFinished();
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
    for (const WordForTraining &wordWithError: m_scatteredLettersStage->wordsWithErrors())
        m_wordsWithErrorsList.insert(wordWithError);
    m_scatteredLettersWordsList = m_scatteredLettersStage->wordsWithErrors();
    if (! m_typeInWordsList.isEmpty())
        typeInStage();
    else if (! m_chooseTranslationWordsList.isEmpty())
        chooseTranslationStage();
    else if (! m_scatteredLettersWordsList.isEmpty())
        scatteredLettersStage();
    else
        allStagesFinished();
}

void Trainer::typeInStage()
{
    m_typeInStage->setWords(m_typeInWordsList);

    removeWidgets();
    layout()->addWidget(m_typeInStage);
    m_typeInStage->setVisible(true);
    m_typeInStage->startStage();
}

void Trainer::typeInStageFinished()
{
    for (const WordForTraining &wordWithError: m_typeInStage->wordsWithErrors())
        m_wordsWithErrorsList.insert(wordWithError);
    m_typeInWordsList = m_typeInStage->wordsWithErrors();

    if (! m_chooseTranslationWordsList.isEmpty())
        chooseTranslationStage();
    else if (! m_scatteredLettersWordsList.isEmpty())
        scatteredLettersStage();
    else if (! m_typeInWordsList.isEmpty())
        typeInStage();
    else
        allStagesFinished();
}

void Trainer::allStagesFinished()
{
    for (auto word: m_wordsList)
        m_vocabulary->updateWord(word.word(), ! m_wordsWithErrorsList.contains(word));

    removeWidgets();
    m_trainingSummary->setStudiedWords(m_wordsList.size() - m_wordsWithErrorsList.size());
    m_trainingSummary->setWordsForRepetition(m_wordsWithErrorsList.size());
    m_trainingSummary->setProgress(
            m_vocabulary->numberOfWordsStudiedToday(),
            m_wordsPerDay);

    layout()->addWidget(m_trainingSummary);
    m_trainingSummary->setVisible(true);
    m_trainingSummary->setFocus();
}

void Trainer::setWords(const QVector<WordForTraining> &wordsList)
{
    m_wordsList = wordsList;
    m_wordWithTranslationWordsList = wordsList;
    m_chooseTranslationWordsList = wordsList;
    m_scatteredLettersWordsList = wordsList;
    m_typeInWordsList = wordsList;
}

void Trainer::removeWidgets()
{
    layout()->removeWidget(m_wordWithTranslationStage);
    layout()->removeWidget(m_chooseTranslationStage);
    layout()->removeWidget(m_typeInStage);
    layout()->removeWidget(m_scatteredLettersStage);
    layout()->removeWidget(m_trainingSummary);
    m_wordWithTranslationStage->setVisible(false);
    m_chooseTranslationStage->setVisible(false);
    m_typeInStage->setVisible(false);
    m_scatteredLettersStage->setVisible(false);
    m_trainingSummary->setVisible(false);
}

void Trainer::loadSettings()
{
    QSettings config;
    m_wordsPerRound = config.value("Trainer/wordsPerRound", 5).toUInt();
    m_wordsPerDay = config.value("Trainer/wordsPerDay", 20).toUInt();
}

}
