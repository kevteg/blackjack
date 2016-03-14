#ifndef NPLAYER_H
#define NPLAYER_H
#include <QObject>
#include <QWidget>
#include "carta.h"
#include <QDebug>

namespace Ui {
class nplayer;
}

class nplayer : public QWidget
{
    Q_OBJECT

public:
    explicit nplayer(QWidget *parent = 0);
    ~nplayer();
    void    setName(QString);
    QString getName();
    void    setId(int);
    int     getId();
    void    setSocketDes(int);
    int     getSocketDes();
    void    playerGone();

    bool    getBonificacion();
    void    setBonificacion(bool );
    int     getPuntos();
    void    setPuntos(int);

    void    addCard(carta new_card);
    void    isMe();
    int     getCartasSum();
    int     getCardsCount();
    int     sumUpPoints();
    void    resetCards();

private slots:
signals:

private:
    Ui::nplayer *ui;
    int id;
    int socket_descriptor;
    int puntos;
    bool bonificacion;
    QString name;
    QVector<carta> cartas;
    QVector<int> puntajes;
    int x;
    int y;
    bool me;
};

#endif // NPLAYER_H
