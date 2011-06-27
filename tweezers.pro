# \file
# <!--
# Tweezers is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
# Copyright 2010 Daniele Basile <asterix@develer.com>
# -->
# \brief Tweezers project.
# \author Daniele Basile <asterix@develer.com>

TARGET = tweezers
TEMPLATE = app

DESTDIR = dist
OBJECTS_DIR = build

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS_DEBUG += -O0 -g3 -ggdb

DEFINES += DEBUG

CONFIG += qt
CONFIG += warn_on
CONFIG += static

SOURCES += main.cpp
SOURCES += fileinfo.cpp
SOURCES += tweezers.cpp
SOURCES += preference.cpp
SOURCES += tag_converter.cpp
SOURCES += list_view.cpp

HEADERS += tweezers.h
HEADERS += itemnode.h
HEADERS += fileinfo.h
HEADERS += tag_converter.h
HEADERS += preference.h
HEADERS += list_view.h

FORMS += tweezers.ui
FORMS += preference.ui

LIBS += -L/usr/local/lib -lexif
INCLUDEPATH += /usr/local/include

RESOURCES += tweezers_res.qrc

OTHER_FILES += images/warning.png
OTHER_FILES += images/ok.png
OTHER_FILES += images/error.png
