/*****************************************************************************
 * ipa.cpp - QStarDict, a dictionary application for learning languages      *
 * Copyright (C) 2017 itinerarium                                            *
 * Copyright (C) 2023 Alexander Rodin                                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation by version 3 of the License.                 *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *****************************************************************************/

#include "ipa.h"

#include <QPair>
#include <QVector>

const QVector<QPair<QRegularExpression, QString>> IPA_TO_KIRSHENBAUM_MAPPING =
{
    { QRegularExpression("^\\s*"), "" },
    { QRegularExpression("\\s*$"), "" },

    { QRegularExpression("(\\.)"), "%" },
    { QRegularExpression("(\u02c8)"), "'" },
    { QRegularExpression("(\u02cc)"), "," },
    { QRegularExpression("(\u0251)"), "A:" },
    { QRegularExpression("(\u02d0)"), ":" },
    { QRegularExpression("(\u0251\u02d0)"), "A" },
    { QRegularExpression("(\u0251\u0279)"), "A" },
    { QRegularExpression("(a\u02d0)"), "A" },

    // feedback from formantzero via r/linguistics
    { QRegularExpression("(\u0329)"), "r" },

    // feedback from scharfes_s via r/linguistics
    { QRegularExpression("(\u027e)"), "t" },

    { QRegularExpression("(\u00e6)"), "a" },
    { QRegularExpression("(a)"), "a" },
    { QRegularExpression("(\u028c)"), "V" },
    { QRegularExpression("(\u0252)"), "0" },
    { QRegularExpression("(\u0254)"), "0" },
    { QRegularExpression("(a\u028a)"), "aU" },
    { QRegularExpression("(\u00e6\u0254)"), "aU" },
    { QRegularExpression("(\u0259)"), "@" },
    { QRegularExpression("(\u025a)"), "3" },
    { QRegularExpression("(\u0259\u02d0)"), "3:" },
    { QRegularExpression("(a\u026a)"), "aI" },
    { QRegularExpression("(\u028c\u026a)"), "aI" },
    { QRegularExpression("(\u0251e)"), "aI" },
    { QRegularExpression("(b)"), "b" },
    { QRegularExpression("(t\u0283)"), "tS" },
    { QRegularExpression("(\u02a7)"), "tS" },
    { QRegularExpression("(d)"), "d" },
    { QRegularExpression("(\u00f0)"), "D" },
    { QRegularExpression("(\u025b)"), "E" },
    { QRegularExpression("(e)"), "E" },
    { QRegularExpression("(\u025d)"), "3:" },
    { QRegularExpression("(\u025c\u02d0)"), "3:" },
    { QRegularExpression("(\u025b\u0259)"), "e@" },
    { QRegularExpression("(e)"), "E" },
    { QRegularExpression("(\u025d)"), "3:" },
    { QRegularExpression("(\u025c\u02d0)"), "3:" },
    { QRegularExpression("(e\u026a)"), "eI" },
    { QRegularExpression("(\u00e6\u026a)"), "eI" },
    { QRegularExpression("(f)"), "f" },
    { QRegularExpression("(\u0261)"), "g" },
    { QRegularExpression("(g)"), "g" },
    { QRegularExpression("(h)"), "h" },
    { QRegularExpression("(\u026a)"), "I" },
    { QRegularExpression("(\u0268)"), "I" },
    { QRegularExpression("(\u026a\u0259)"), "i@" },
    { QRegularExpression("(\u026a\u0279)"), "i@" },
    { QRegularExpression("(\u026a\u0279\u0259)"), "i@3" },
    { QRegularExpression("(i)"), "i:" },
    { QRegularExpression("(i\u02d0)"), "i:" },
    { QRegularExpression("(d\u0292)"), "dZ" },
    { QRegularExpression("(\u02a4)"), "dZ" },
    { QRegularExpression("(k)"), "k" },
    { QRegularExpression("(x)"), "x" },
    { QRegularExpression("(l)"), "l" },
    { QRegularExpression("(d\u026b)"), "l" },
    { QRegularExpression("(m)"), "m" },
    { QRegularExpression("(n)"), "n" },
    { QRegularExpression("(\u014b)"), "N" },
    { QRegularExpression("(\u0259\u028a)"), "oU" },
    { QRegularExpression("(o)"), "oU" },
    { QRegularExpression("(o\u028a)"), "oU" },
    { QRegularExpression("(\u0259\u0289)"), "V" },
    { QRegularExpression("(\u0254\u026a)"), "OI" },
    { QRegularExpression("(o\u026a)"), "OI" },
    { QRegularExpression("(p)"), "p" },
    { QRegularExpression("(\u0279)"), "r" },
    { QRegularExpression("(s)"), "s" },
    { QRegularExpression("(\u0283)"), "S" },
    { QRegularExpression("(t)"), "t" },
    { QRegularExpression("(\u027e)"), "t" },
    { QRegularExpression("(\u03b8)"), "T" },
    { QRegularExpression("(\u028a\u0259)"), "U@" },
    { QRegularExpression("(\u028a\u0279)"), "U@" },
    { QRegularExpression("(\u028a)"), "U" },
    { QRegularExpression("(\u0289\u02d0)"), "u:" },
    { QRegularExpression("(u\u02d0)"), "u:" },
    { QRegularExpression("(u)"), "u:" },
    { QRegularExpression("(\u0254\u02d0)"), "O:" },
    { QRegularExpression("(o\u02d0)"), "O:" },
    { QRegularExpression("(v)"), "v" },
    { QRegularExpression("(w)"), "w" },
    { QRegularExpression("(\u028d)"), "w" },
    { QRegularExpression("(j)"), "j" },
    { QRegularExpression("(z)"), "z" },
    { QRegularExpression("(\u0292)"), "Z" },
    { QRegularExpression("(\u0294)"), "?" },

    // special edits
    { QRegularExpression("(k\'a2n)"), "k\'@n" },
    { QRegularExpression("(ka2n)"), "k@n" },
    { QRegularExpression("(gg)"), "g" },
    { QRegularExpression("(@U)"), "oU" },
    { QRegularExpression("rr$"), "r" },
    { QRegularExpression("3r$"), "3:" },
    { QRegularExpression("([iU]|([AO]:))@r$"), "\\1@" },
    { QRegularExpression("([^e])@r"), "\\1:3" },
    { QRegularExpression("e@r$"), "e@" },
    { QRegularExpression("e@r([bdDfghklmnNprsStTvwjzZ])"), "e@\\1" },

    // edits arising from testing
    { QRegularExpression("('k)+"), "k'" },
    { QRegularExpression("(ː)+"), ":" },
    { QRegularExpression("(:)+"), ":" },
    { QRegularExpression("(ᵻ)"), "I" },
    { QRegularExpression("(ɜ)"), "3" },
    { QRegularExpression("(ɔ)"), "O" },

    // feedback from formantzero via r/linguistics
    { QRegularExpression("\u0361(.)"), "\\1\'" },
    { QRegularExpression("3$"), "R" },

    // QStarDict modifications
    { QRegularExpression("ı"), "i" }
};

const QString IPA_REGEXP =
    "["
    "'"
    "ˌ"
    "\\."
    "\\("
    "\\)"
    "a"
    "b"
    "d"
    "e"
    "f"
    "g"
    "h"
    "i"
    "j"
    "k"
    "l"
    "m"
    "n"
    "o"
    "p"
    "r"
    "s"
    "t"
    "u"
    "v"
    "w"
    "x"
    "z"
    "\u00e6"
    "\u00f0"
    "\u014b"
    "\u0251"
    "\u0251"
    "\u0252"
    "\u0254"
    "\u0259"
    "\u025a"
    "\u025b"
    "\u025c"
    "\u025d"
    "\u0261"
    "\u0268"
    "\u026a"
    "\u026b"
    "\u0279"
    "\u027e"
    "\u0283"
    "\u0289"
    "\u028a"
    "\u028c"
    "\u028d"
    "\u0292"
    "\u0294"
    "\u02a4"
    "\u02a7"
    "\u02c8"
    "\u02cc"
    "\u02d0"
    "\u0329"
    "\u0361"
    "\u03b8"
    "\u1d7b"
    "ı"
    "]+"
;

QString Ipa::ipaToKirshenbaum(QString ipa)
{
    for (const QPair<QRegularExpression, QString> &mapping: IPA_TO_KIRSHENBAUM_MAPPING)
        ipa.replace(mapping.first, mapping.second);
    return ipa;
}

QRegularExpression Ipa::narrowTranscriptionRegExp()
{
    return QRegularExpression("(\\/(" + IPA_REGEXP + ")\\/)");
}

QRegularExpression Ipa::broadTranscriptionRegExp()
{
    return QRegularExpression("(\\[(" + IPA_REGEXP + ")\\])");
}
