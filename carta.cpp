#include "carta.h"

carta::carta(QString nombre) : nombre(nombre){
   // this->imagen = new image(QPixmap(":/imágenes/Imágenes/Baraja/" + nombre + ".jpg"), 200, 200);
}

QString carta::getNombre(){
    return nombre;
}

image *carta::getImage(){
    return imagen;
}
