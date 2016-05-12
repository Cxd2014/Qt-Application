#-------------------------------------------------
#
# Project created by QtCreator 2016-03-20T14:52:41
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Router
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    setting.cpp \
    qextserialbase.cpp \
    win_qextserialport.cpp

HEADERS  += widget.h \
    qextserialbase.h \
    win_qextserialport.h

FORMS    += widget.ui
