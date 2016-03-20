#-------------------------------------------------
#
# Project created by QtCreator 2015-06-23T14:16:33
#
#-------------------------------------------------

QT       += core gui
QT	 += sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt5_rfid
TEMPLATE = app

RC_FILE = ICON.rc

SOURCES += main.cpp\
        widget.cpp \
    LogIn.cpp \
    Rfid.cpp \
    qextserialbase.cpp \
    win_qextserialport.cpp

HEADERS  += widget.h \
    qextserialbase.h \
    win_qextserialport.h

FORMS    += widget.ui

RESOURCES += \
    src.qrc
