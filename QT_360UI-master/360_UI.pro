#-------------------------------------------------
#
# Project created by QtCreator 2015-07-12T20:49:35
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 360_UI
TEMPLATE = app
RC_FILE = ICON.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    menubutton.cpp

HEADERS  += mainwindow.h \
    menubutton.h

FORMS    += mainwindow.ui

RESOURCES += \
    src.qrc
