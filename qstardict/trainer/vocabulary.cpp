/*****************************************************************************
 * vocabulary.cpp - QStarDict, a dictionary for learning languages           *
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

#include "vocabulary.h"

#include <QDir>
#include <QMessageBox>
#include <QSqlQuery>

namespace QStarDict
{

Vocabulary::Vocabulary(const QString &vocabularyName)
{
    m_vocabularyName = vocabularyName;
    m_db = QSqlDatabase::addDatabase("QSQLITE", vocabularyName);

    QDir databaseDir = QDir::homePath() + "/.qstardict/training-vocabulary";
    if (! databaseDir.exists())
        databaseDir.mkpath(".");
    QString databaseFilename = databaseDir.absolutePath() + "/" + vocabularyName + ".sqlite3";
    m_db.setDatabaseName(databaseFilename);
    if (! m_db.open())
    {
        QMessageBox::critical(nullptr, QObject::tr("Database error"),
                QObject::tr("Cannot open vocabulary database at %1").arg(databaseFilename));
        return;
    }

    QSqlQuery query(m_db);
    query.exec("CREATE TABLE IF NOT EXISTS words ("
            "word TEXT PRIMARY KEY, "
            "translation TEXT, "
            "transcription TEXT, "
            "studied INTEGER, "
            "lastExcersise TEXT"
            ")");
}

Vocabulary::~Vocabulary()
{
}

void Vocabulary::addWord(const WordForTraining &word)
{

}

};
