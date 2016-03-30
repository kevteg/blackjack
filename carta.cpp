#include "carta.h"

carta::carta(QString nombre){
    QString ini = nombre.split(nombre.right(1))[0];
    QString tipo = nombre.right(1);
    this->imagen = new image(QPixmap(":/imágenes/Imágenes/Baraja/" + ini.toUpper() + tipo.toLower() + ".png"), 150, 150);
    this->nombre = ini.toUpper() + tipo.toLower();
    type_value = (tipo == "p")?4:((tipo == "t")?3:0);
    num_value = (ini.toUpper() == "Q" || ini.toUpper() == "J" || ini.toUpper() == "K" || ini.toUpper() == "10")?2:((ini.toUpper() == "A")?1:0);
    //Como el valor del as varia le pondre -1 si la carta retorna eso entonces es as
    val = ini.toInt()?ini.toInt():((ini.toUpper() == "Q" || ini.toUpper() == "J" || ini.toUpper() == "K")?10:-1);
    qDebug() << "Valor: " << val;
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
