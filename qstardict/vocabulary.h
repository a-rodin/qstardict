/*****************************************************************************
 * vocabulary.h - QStarDict, a StarDict clone written using Qt               *
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

#ifndef VOCABULARY_H
#define VOCABULARY_H

#include <QString>
#include <QSqlDatabase>

namespace QStarDict
{

class Vocabulary
{
    public:
        Vocabulary();
        virtual ~Vocabulary();

        /**
         * Add a new word to the vocabulary. If a word already exists, the translation and transcription
         * are updated and the "studied" field is reset.
         */
        void addWord(const QString &word, const QString &translation, const QString &transcription);

        /**
         * Return n words for studying.
         */
        QVector<WordForTraining> getWordsForTraining(unsigned n);

        // void markAsStudied(const QString &word);

    private:
        QSqlDatabase m_db;
};

}

#endif // VOCABULARY_H

