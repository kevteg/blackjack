#include "carta.h"

carta::carta(QString nombre) : nombre(nombre){
    this->imagen = new image(QPixmap(":/imágenes/Imágenes/Baraja/" + nombre + ".png"), 150, 150);
    QString ini = nombre.split(nombre.right(1))[0];
    QString tipo = nombre.right(1);
    type_value = (tipo == "p")?4:((tipo == "t")?3:0);
    num_value = (ini == "Q" || ini == "J" || ini == "K" || ini == "10")?2:((ini == "A")?1:0);
    //Como el valor del as varia le pondre -1 si la carta retorna eso entonces es as
    val   = ini.toInt()?ini.toInt():((ini == "Q" || ini == "J" || ini == "K")?10:-1);
}
int carta::getTypeValue(){
    return type_value;
}

int carta::getNumValue(){
    return num_value;
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
