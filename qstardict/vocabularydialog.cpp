/*****************************************************************************
 * vocabularydialog.cpp - QStarDict, a dictionary for learning languages     *
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

#include "vocabularydialog.h"

#include "application.h"
#include "vocabulary.h"

#include <QInputDialog>
#include <QSqlTableModel>

namespace QStarDict
{

VocabularyDialog::VocabularyDialog(QWidget *parent)
    : QDialog(parent),
      m_tableModel(nullptr)
{
    setupUi(this);
    loadVocabulary();
}

VocabularyDialog::~VocabularyDialog()
{
    delete m_tableModel;
}

void VocabularyDialog::on_removeWordButton_clicked()
{
    QItemSelection selection = wordsView->selectionModel()->selection();
    for (QModelIndex index: selection.indexes())
        m_tableModel->removeRow(index.row());
    loadVocabulary();
}

void VocabularyDialog::loadVocabulary()
{
    delete m_tableModel;
    m_tableModel = new QSqlTableModel(nullptr, Application::instance()->vocabulary()->m_db);
    m_tableModel->setTable("words");
    m_tableModel->select();
    wordsView->setModel(m_tableModel);
}

}
