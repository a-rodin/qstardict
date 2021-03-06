#############################################################################
# swac.pro                                                                  #
# This file is a part of QStarDict, a StarDict clone written with using Qt  #
# swac.cpp - Plugin for words audio collections SWAC                        #
# Copyright (C) 2008 Nicolas Vion <nico@picapo.net>                         #
#                                                                           #
# This program is free software; you can redistribute it and/or modify      #
# it under the terms of the GNU General Public License as published by      #
# the Free Software Foundation; either version 2 of the License, or         #
# (at your option) any later version.                                       #
#                                                                           #
# This program is distributed in the hope that it will be useful,           #
# but WITHOUT ANY WARRANTY; without even the implied warranty of            #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             #
# GNU General Public License for more details.                              #
#                                                                           #
# You should have received a copy of the GNU General Public License along   #
# with this program; if not, write to the Free Software Foundation, Inc.,   #
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.               #
#############################################################################

TARGET = swac
META_VERSION = 0.2
META_NAME = swac
META_DESC = "An experimental plugin for words audio collections (SWAC).<br>For more information about SWAC, please, visit the <a href='http://shtooka.net/'>Shtooka Project Homepage</a>."
META_AUTHORS = Nicolas Vion <nico@picapo.net>
META_FEATURES = dict;defenable


include(../plugin.pri)

QT += sql

HEADERS += \
    swac.h

SOURCES += \
    swac.cpp

RESOURCES += \
    pixmaps/pixmaps.qrc

OTHER_FILES += \
    swac.json

