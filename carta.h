#ifndef CARTA_H
#define CARTA_H
#include "image.h"
#include <QDebug>
class carta
{
    //Q_OBJECT
public:
    //Si se cuenta algo de la carta podria estar ese número aqui
    carta(QString nombre = "");
    QString getNombre();
    image *getImage();
    int getValue();
    int operator+(carta);
private:
    image *imagen;
    QString nombre;
    int val;
};

#endif // CARTA_H
