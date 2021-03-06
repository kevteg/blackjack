#ifndef PANEL_JUEGO_H
#define PANEL_JUEGO_H

#include <QWidget>
#include "panel_juego.h"
#include "nplayer.h"
namespace Ui {
class panel_juego;
}

class panel_juego : public QWidget
{
    Q_OBJECT

public:
    explicit panel_juego(QWidget *parent = 0);
    ~panel_juego();
    void newPlayer();
    int getPlayersCount(bool is_server);
    void addPlayer(bool isPlayer, nplayer *new_player);
    void changeBarajaValue(int bar);
    void changeRondaValue();
    int getRondaValue();
private slots:
    void goBack();

signals:
    void returnToInit();
private:
    int players;
    Ui::panel_juego *ui;
    int rounds;
};

#endif // PANEL_JUEGO_H
