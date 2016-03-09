#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QDirIterator>
#include "nplayer.h"
#include "panel_juego.h"
#include "protocolo.h"
#include "image.h"
#include "carta.h"
class game : public QObject
{
    Q_OBJECT
public:
    explicit game(int tipo_juego, QObject *parent = 0);
    void setPanel(panel_juego *panel);
    void setJugadores(QVector<nplayer*> *jugadores);
    void beginGame();
    void llenarBaraja();
    carta *getRandomCard();
    carta *getRandomUsedCard();
signals:

public slots:
private:
    int tipo_juego;
    panel_juego *panel;
    QVector<carta*> baraja;
    QVector<carta*> cartas_usadas;
    QVector<nplayer*> *jugadores;
};

#endif // GAME_H
