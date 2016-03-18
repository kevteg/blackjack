#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QTimer>
#include <QTime>
#include <QDirIterator>
#include "nplayer.h"
#include "panel_juego.h"
#include "protocolo.h"
#include "image.h"
#include "carta.h"
#include "panel_estadisticas.h"
class game : public QObject
{
    Q_OBJECT
public:
    explicit game(int tipo_juego, QObject *parent = 0);
    void setPanel(panel_juego *panel);
    void setJugadores(QVector<nplayer*> *jugadores);
    void beginGame();
    void llenarBaraja();
    carta getRandomCard();
    carta getRandomUsedCard();
    bool beginRound();
    void bonification(int socketDes);
    bool prestamo;
    void enviarRonda();
    void cardInfo(int id, carta card);
    void takeOffCard(carta ncarta);
    void cardOffering();
    void cardReply(int socket_des, bool resp);
    void sendCardToTurnPlayer();
    void stopGame();
    void renewRound();
    void setPlayersPoints(QVector<int> *ids, QVector<int> *points);
    void finishGame();
    bool desempateFinal();
    void finishClientGame(int cards, int rounds, bool empate, QVector<int> *ids, QVector<int> *points);

signals:
    void sendUnicast(int, int, QVector<QVariant>, int  var = 0);
    void sendMulticast(int, QVector<QVariant>);
    void goInit();


public slots:
    void verifyStatus();
    void goInitInterface();
private:
    int tipo_juego;
    int round_count;
    int turno;
    int status;
    int ignore;
    panel_estadisticas *panel_es;
    QTimer *vertimer;
    panel_juego *panel;
    QVector<carta> baraja;
    QVector<carta> cartas_usadas;
    QVector<nplayer*> *jugadores;
    QVector <nplayer*>::iterator turn_player;
    QVector <nplayer*>::iterator beginner_player;
    bool empate;
};

#endif // GAME_H
