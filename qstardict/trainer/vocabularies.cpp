/*****************************************************************************
 * vocabularies.cpp - QStarDict, a dictionary for learning languages         *
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

 #include "vocabularies.h"

 #include <QDir>

namespace QStarDict
{

Vocabularies::~Vocabularies()
{
    for (Vocabulary *vocabulary: m_vocabularies.values())
        delete vocabulary;
}

QStringList Vocabularies::vocabulariesList() const
{
    QStringList files = QDir(QDir::homePath() + "/.qstardict/training-vocabulary").entryList({"*.sqlite3"});

    QStringList vocabularies;
    for (QString file: files) {
        vocabularies << file.replace(".sqlite3", "");
    }

    return vocabularies;
}

void Vocabularies::addVocabulary(const QString &vocabularyName)
{
    Vocabulary *vocabulary = new Vocabulary(vocabularyName);
    m_vocabularies[vocabularyName] = vocabulary;
}

Vocabulary *Vocabularies::vocabulary(const QString &vocabularyName)
{
    if (vocabulariesList().contains(vocabularyName))
    {
        if (! m_vocabularies.contains(vocabularyName))
            addVocabulary(vocabularyName);

        return m_vocabularies[vocabularyName];
    }
    return nullptr;
}

}
