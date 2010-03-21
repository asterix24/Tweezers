# \file
# <!--
# Tweezers is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
# Copyright 2010 Daniele Basile <asterix@develer.com>
# -->
# \brief Tweezers project.
# \author Daniele Basile <asterix@develer.com>
TARGET = tweezers
TEMPLATE = app
SOURCES += main.cpp \
    tweezers.cpp \
    tag_converter.cpp
HEADERS += tweezers.h \
    tag_converter.h
FORMS += tweezers.ui \
    preference.ui
LIBS += -Llib/lib \
    -lexiv2
INCLUDEPATH += lib/include
