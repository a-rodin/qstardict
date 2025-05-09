/*****************************************************************************
 * ipaspeaker.h - QStarDict, a dictionary application for learning           *
 *                languages                                                  *
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

#ifndef IPASPEAKER_H
#define IPASPEAKER_H

#include <QString>
#include "speaker.h"

namespace QStarDict
{

class IpaSpeaker
{
    public:
        IpaSpeaker();
        virtual ~IpaSpeaker();

        void speak(const QString &transcription);

        void setSpeechCmd(const QString &cmd)
        { m_espeakSpeaker->setSpeechCmd(cmd); }
        const QString &speechCmd() const
        { return m_espeakSpeaker->speechCmd(); }

    private:
        Speaker *m_espeakSpeaker;
};

}

#endif // IPASPEAKER_H

