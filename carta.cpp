#include "carta.h"

carta::carta(QString nombre) : nombre(nombre){
    this->imagen = new image(QPixmap(":/imágenes/Imágenes/Baraja/" + nombre + ".png"), 150, 150);
    QString ini = nombre.split(nombre.right(1))[0];
    //Como el valor del az varia le pondre -1 si la carta retorna eso entonces es az
    val = ini.toInt()?ini.toInt():((ini == "Q" || ini == "J" || ini == "K")?10:-1);
}

int carta::getValue(){
    return val;
}
int carta::operator+(carta ncarta){
    return this->val + ncarta.getValue();
}

QString carta::getNombre(){
    return nombre;
}

image *carta::getImage(){
    return imagen;
}
