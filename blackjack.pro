#-------------------------------------------------
#
# Project created by QtCreator 2016-02-13T13:40:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = blackjack
TEMPLATE = app


SOURCES += main.cpp\
        blackmain.cpp \
    image.cpp \
    MultipurposeSocket_ReadyToUse/MultiPropuseSockets.cpp \
    comunication.cpp

HEADERS  += blackmain.h \
    image.h \
    MultipurposeSocket_ReadyToUse/MultiPropuseSockets.h \
    comunication.h

FORMS    += blackmain.ui
QT       += network

RESOURCES += \
    imagenes.qrc
