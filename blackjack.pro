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
    communication.cpp \
    servidor.cpp \
    interfaz_inicial.cpp

HEADERS  += blackmain.h \
    image.h \
    communication.h \
    servidor.h \
    interfaz_inicial.h

FORMS    += blackmain.ui \
    servidor.ui \
    interfaz_inicial.ui
QT       += network

RESOURCES += \
    imagenes.qrc
