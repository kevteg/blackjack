#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "nplayer.h"
#include "panel_juego.h"
#include "protocolo.h"
#include "image.h"

class game : public QObject
{
    Q_OBJECT
public:
    explicit game(int tipo_juego, QObject *parent = 0);
    void setPanel(panel_juego *panel);
    void setJugadores(QVector<nplayer*> *jugadores);
    void beginGame();
signals:

public slots:
private:
    int tipo_juego;
    panel_juego *panel;
    QVector<nplayer*> *jugadores;

};

#endif // GAME_H
