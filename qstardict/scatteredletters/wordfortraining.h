/*****************************************************************************
 * wordfortraining.h - QStarDict, a free dictionary application              *
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

#ifndef WORDFORTRAINING_H
#define WORDFORTRAINING_H

class WordForTraining
{
    public:
        WordForTraining(const QString &word, const QString &translation)
            : m_word(word), m_translation(translation)
        { }

        QString word() const
        { return m_word; }

        QString translation() const
        { return m_translation; }

    private:
        QString m_word;
        QString m_translation;
};

#endif // WORDFORTRAINING_H