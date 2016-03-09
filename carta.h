#ifndef CARTA_H
#define CARTA_H
#include "image.h"

class carta
{
public:
    //Si se cuenta algo de la carta podria estar ese número aqui
    carta(QString nombre);
    QString getNombre();
    image *getImage();
private:
    image *imagen;
    QString nombre;
};

#endif // CARTA_H
