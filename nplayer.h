#ifndef NPLAYER_H
#define NPLAYER_H
#include <QObject>
#include <QWidget>
#include "image.h"

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
private slots:
signals:

private:
    Ui::nplayer *ui;
    int id;
    int socket_descriptor;
    int puntos;
    QString name;
};

#endif // NPLAYER_H
