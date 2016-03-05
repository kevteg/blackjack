#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

class player : public QObject
{
    Q_OBJECT
public:
    explicit player(QObject *parent = 0);
    void    setName(QString);
    QString getName();
    void    setId(int);
    int     getId();
    void    setSocketDes(int);
    int     getSocketDes();


signals:

public slots:
private:
    int id;
    int socket_descriptor;
    int puntos;
    QString name;

};

#endif // PLAYER_H
