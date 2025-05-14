/*****************************************************************************
 * vocabulary.cpp - QStarDict, a dictionary application for learning         *
 *                  languages                                                *
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
            "last_excercise TEXT"
            ")");
}

Vocabulary::~Vocabulary()
{
    m_db.close();
}

void Vocabulary::addWord(const QString &word, const QString &translation, const QString &transcription)
{
    QSqlQuery query(m_db);
    query.prepare(
            "INSERT INTO words\n"
            "    (word, translation, transcription, studied)\n"
            "    VALUES (:word, :translation, :transcription, 0)\n"
            "    ON CONFLICT (word)\n"
            "    DO UPDATE SET translation = :translation, transcription = :transcription, studied = 0;");
    query.bindValue(":word", word);
    query.bindValue(":translation", translation);
    query.bindValue(":transcription", transcription);
    if (! query.exec())
        qDebug() << query.lastError();
}

QVector<WordForTraining> Vocabulary::getWordsForTraining(unsigned n)
{
    QSqlQuery query(m_db);
    query.prepare(
            "SELECT DISTINCT word, translation, transcription\n"
            "FROM words\n"
            "ORDER BY studied, random()\n"
            "LIMIT :limit\n");
    query.bindValue(":limit", n);
    if (! query.exec())
    {
        qDebug() << "getWordsForTraining error: " << query.lastError();
        return {};
    }

    QVector<WordForTraining> result;
    while (query.next())
    {
        QString word = query.value("word").toString();
        QString translation = query.value("translation").toString();
        QString transcription = query.value("transcription").toString();

        WordForTraining wordForTraining(word, translation, transcription);
        result << wordForTraining;
    }

    return result;
}

QStringList Vocabulary::getRandomTranslations(unsigned n, const QStringList &skipList)
{
    QStringList escapedSkipList;
    for (auto translation: skipList)
        escapedSkipList << "\"" + translation.replace("\"", "\\\"") + "\"";
    QSqlQuery query(m_db);
    QString queryString = QString(
            "SELECT translation FROM words\n"
            "WHERE translation NOT IN (%1)\n"
            "ORDER BY random()\n"
            "LIMIT :limit").arg(escapedSkipList.join(", "));
    query.prepare(queryString);
    query.bindValue(":limit", n);
    if (! query.exec())
    {
        qDebug() << "getRandomTranslations error: " << query.lastError();
        return {};
    }

    QStringList result;
    while (query.next())
        result << query.value("translation").toString();

    return result;
}

void Vocabulary::updateWord(const QString &word, bool studied)
{
    QSqlQuery query(m_db);
    query.prepare(
        "UPDATE words\n"
        "SET studied = studied + :studied, last_excercise = datetime()\n"
        "WHERE word = :word");
    query.bindValue(":studied", studied ? 1 : 0);
    query.bindValue(":word", word);
    if (! query.exec())
        qDebug() << "updateWord error: " << query.lastError();
}

unsigned Vocabulary::numberOfWordsStudiedToday()
{
    QSqlQuery query(m_db);
    query.prepare(
        "SELECT count() FROM words\n"
        "WHERE studied AND strftime(\"%s\", datetime()) - strftime(\"%s\", last_excercise) < 60 * 60 * 24");
    if (! query.exec())
        qDebug() << "numberOfWordsStudiedToday error: " << query.lastError();

    if (query.next())
        return query.value(0).toUInt();
    else
        return 0;
}

}
