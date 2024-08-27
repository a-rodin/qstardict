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

QStringList Vocabularies::vocabulariesList() const
{
    QStringList files = QDir(QDir::homePath() + "/.qstardict/training-vocabulary").entryList({"*.sqlite3"});

    QStringList vocabularies;
    for (QString file: files) {
        vocabularies << file.replace(".sqlite3", "");
    }

    return vocabularies;
}

bool Vocabularies::addVocabulary(const QString &vocabularyName)
{
    return false;
}

Vocabulary *Vocabularies::vocabulary(const QString &vocabularyName)
{
    return nullptr;
}

}
