/*****************************************************************************
 * vocabularydialog.cpp - QStarDict, a dictionary for learning languages     *
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

#include "vocabularydialog.h"

#include "../application.h"
#include "addworddialog.h"
#include "vocabularies.h"

#include <QInputDialog>
#include <QDebug>

namespace QStarDict
{

VocabularyDialog::VocabularyDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    reloadVocabularies();
}

VocabularyDialog::~VocabularyDialog()
{

}

void VocabularyDialog::on_addVocabularyButton_clicked()
{
    bool ok;
    QString vocabularyName = QInputDialog::getText(this, tr("Vocabulary name"),
        tr("Enter the vocabulary name"),
        QLineEdit::Normal,
        QString(),
        &ok);
    if (ok && ! vocabularyName.isEmpty())
    {
        Application::instance()->vocabularies()->addVocabulary(vocabularyName);
        reloadVocabularies();
        vocabularyComboBox->setCurrentText(vocabularyName);
    }
}

void VocabularyDialog::on_addWordButton_clicked()
{
    AddWordDialog addWordDialog(this);
    int result = addWordDialog.exec();
    if (result == QDialog::Accepted)
    {
        WordForTraining wordForTraining = addWordDialog.getWordForTraining();

        QString vocabularyName = vocabularyComboBox->currentText();
        Vocabulary *vocabulary = Application::instance()->vocabularies()->vocabulary(vocabularyName);
        vocabulary->addWord(wordForTraining);
    }
}

void VocabularyDialog::reloadVocabularies()
{
    vocabularyComboBox->clear();
    vocabularyComboBox->addItems(Application::instance()->vocabularies()->vocabulariesList());
}

}
