/*****************************************************************************
 * ipa.h - QStarDict, a dictionary application for learning languages        *
 * Copyright (C) 2023 Alexander Rodin                                        *
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


#ifndef IPA_H
#define IPA_H

#include <QString>
#include <QRegularExpression>

class Ipa
{
    public:
        /**
         * Convert a transcription in International Phonetic Alphabet into Kirshenbaum
         * notation acceptable by espeak.
         */
        static QString ipaToKirshenbaum(QString ipa);

        /**
         * Return a regular expression which detects transcriptions in form /.../.
         * The second capture is the expression inside the brackets.
         */
        static QRegularExpression narrowTranscriptionRegExp();

        /**
         * Return a regular expression which detects transcriptions in form [...].
         * The second capture is the expression inside the brackets.
         */
        static QRegularExpression broadTranscriptionRegExp();
};

#endif // IPA_H
