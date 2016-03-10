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
<<<<<<< 423e18ac7279f2f4de2b67cafa488e675dd020db
    bool    getBonificacion();
    void    setBonificacion(bool );
    int     getPuntos();
    void    setPuntos(int);
=======
    void    addCard(carta new_card);
>>>>>>> 21e0472f42ce709121271f09b28a645990c6098f
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
};

#endif // NPLAYER_H
