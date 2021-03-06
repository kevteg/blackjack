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
    interfaz_inicio.cpp \
    interfaz_servidor.cpp \
    interfaz_cliente.cpp \
    client_thread.cpp \
    client.cpp \
    datasend.cpp \
    server.cpp \
    udpcomu.cpp \
    panel_juego.cpp \
    serveritem.cpp \
    nplayer.cpp \
    game.cpp \
    carta.cpp \
    item_lista.cpp \
    panel_estadisticas.cpp

HEADERS  += blackmain.h \
    image.h \
    interfaz_inicio.h \
    interfaz_servidor.h \
    interfaz_cliente.h \
    client_thread.h \
    client.h \
    datasend.h \
    server.h \
    udpcomu.h \
    panel_juego.h \
    protocolo.h \
    serveritem.h \
    nplayer.h \
    game.h \
    carta.h \
    item_lista.h \
    panel_estadisticas.h

FORMS    += blackmain.ui \
    interfaz_inicio.ui \
    interfaz_servidor.ui \
    interfaz_cliente.ui \
    panel_juego.ui \
    serveritem.ui \
    nplayer.ui \
    item_lista.ui \
    panel_estadisticas.ui
QT       += network

RESOURCES += \
    imagenes.qrc
