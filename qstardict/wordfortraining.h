/*****************************************************************************
 * wordfortraining.h - QStarDict, a dictionary application for learning      *
 *                     languages                                             *
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

#ifndef WORDFORTRAINING_H
#define WORDFORTRAINING_H

#include <QHash>

namespace QStarDict
{

class WordForTraining
{
    public:
        WordForTraining(const QString &word, const QString &translation, const QString &transcription)
            : m_word(word), m_translation(translation), m_transcription(transcription)
        { }

        QString word() const
        { return m_word; }

        QString translation() const
        { return m_translation; }

        QString transcription() const
        { return m_transcription; }

        friend bool operator ==(const WordForTraining &first, const WordForTraining &second);
        friend size_t qHash(const WordForTraining &word, size_t seed);

    private:
        QString m_word;
        QString m_translation;
        QString m_transcription;
};

inline bool operator ==(const WordForTraining &first, const WordForTraining &second)
{
    return first.m_word == second.m_word &&
            first.m_translation == second.m_translation &&
            first.m_transcription == second.m_transcription;
}

inline size_t qHash(const WordForTraining &word, size_t seed = 0)
{ return qHashMulti(seed, word.m_word, word.m_translation, word.m_transcription); }

}

#endif // WORDFORTRAINING_H
