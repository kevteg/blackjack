#ifndef PANEL_JUEGO_H
#define PANEL_JUEGO_H

#include <QWidget>
#include "panel_juego.h"
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
private slots:
    void goBack();

signals:
    void returnToInit();
private:
    Ui::panel_juego *ui;
};

#endif // PANEL_JUEGO_H
