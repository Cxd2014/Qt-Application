#-------------------------------------------------
#
# Project created by QtCreator 2015-07-31T16:42:37
#
#-------------------------------------------------

QT       += core gui
QT       += network

TARGET = Router
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    Setting.cpp \
    qextserialport.cpp \
    qextserialport_unix.cpp

HEADERS  += widget.h \
    qextserialport.h \
    qextserialport_p.h \
    qextserialport_global.h

FORMS    += widget.ui

RESOURCES += \
    src/src.qrc
