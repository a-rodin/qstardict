/*****************************************************************************
 * vocabulary.cpp - QStarDict, a StarDict clone written using Qt             *
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

#include "vocabulary.h"

#include <QDir>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

namespace QStarDict
{

Vocabulary::Vocabulary()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", "vocabulary");

    QDir databaseDir = QDir::homePath() + "/.qstardict";
    if (! databaseDir.exists())
        databaseDir.mkpath(".");
    QString databaseFilename = databaseDir.absolutePath() + "/vocabulary.sqlite3";

    m_db.setDatabaseName(databaseFilename);

    if (! m_db.open())
    {
        QMessageBox::critical(nullptr, QObject::tr("Database error"),
                QObject::tr("Cannot open vocabulary database at %1").arg(databaseFilename));
        return;
    }

    QSqlQuery query(m_db);
    query.exec(
            "CREATE TABLE IF NOT EXISTS words (\n"
            "word TEXT PRIMARY KEY,\n"
            "translation TEXT,\n"
            "transcription TEXT,\n"
            "studied INTEGER,\n"
            "lastExcersise TEXT"
            ")");
}


void Vocabulary::addWord(const QString &word, const QString &translation, const QString &transcription)
{
    QSqlQuery query(m_db);
    query.prepare(
            "INSERT INTO words\n"
            "    (word, translation, transcription)\n"
            "    VALUES (:word, :translation, :transcription)\n"
            "    ON CONFLICT (word)\n"
            "    DO UPDATE SET translation = :word, transcription = :transcription, studied = NULL;");
    query.bindValue(":word", word);
    query.bindValue(":translation", translation);
    query.bindValue(":transcription", transcription);
    if (! query.exec())
    {
        qDebug() << query.lastError();
    }
}

}
