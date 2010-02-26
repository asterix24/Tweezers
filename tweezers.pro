# -------------------------------------------------
# Project created by QtCreator 2010-01-24T16:33:28
# -------------------------------------------------

TARGET = tweezers

TEMPLATE = app

SOURCES += main.cpp \
    tweezers.cpp \
    tag_converter.cpp

HEADERS += tweezers.h \
    lib/exv_conf.h \
    tag_converter.h

FORMS += tweezers.ui

LIBS += -Llib/src

INCLUDEPATH += -Ilib/src \
               -Ilib
