#ifndef PLAYER_H
#define PLAYER_H
#include <QString>

class player
{

public:
    player();
    void    setName(QString);
    QString getName();
    void    setId(int);
    int     getId();
    void    setSocketDes(int);
    int     getSocketDes();

private:
    int id;
    int socket_descriptor;
    int puntos;
    QString name;

};

#endif // PLAYER_H
