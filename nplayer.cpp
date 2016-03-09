#include "nplayer.h"
#include "ui_nplayer.h"

nplayer::nplayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::nplayer)
{
    ui->setupUi(this);
    ui->puntos->setText("0");
    this->setStyleSheet("QGroupBox:border{none}\
                         QGroupBox{\
                         background: #B2EBF2;\
                         }");
}

nplayer::~nplayer()
{
    delete ui;
}
QString nplayer::getName(){
    return name;
}
void nplayer::setName(QString name){
    this->name= name;
    ui->nombre->setText(this->name);
}
void nplayer::setId(int id){
    this->id= id;
}
int nplayer::getId(){
    return id;
}
void nplayer::setSocketDes(int socket_descriptor){
    this->socket_descriptor= socket_descriptor;
}
int nplayer::getSocketDes(){
    return socket_descriptor;
}
void nplayer::playerGone(){
    this->setStyleSheet("QGroupBox{\
                         background: #757575;\
                         }");
}

bool nplayer::getBonificacion(){
    return bonificacion;
}
void nplayer::setBonificacion(bool bonificacion){
    this->bonificacion= bonificacion;
}
int  nplayer::getPuntos(){
    return puntos;
}
void nplayer::setPuntos(int puntos){
    this->puntos=puntos;
}
