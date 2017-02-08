#-------------------------------------------------
#
# Project created by QtCreator 2017-02-05T16:08:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TimeConsume
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    timeunit.cpp \
    timecore.cpp

HEADERS  += widget.h \
    timeunit.h \
    timecore.h

FORMS    += widget.ui

LIBS += -LC:/Qt/Tools/mingw530_32/i686-w64-mingw32/include -lpsapi

RC_FILE = res.rc

DISTFILES += \
    update.log \
    res.rc
