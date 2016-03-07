#include "nplayer.h"
#include "ui_nplayer.h"

nplayer::nplayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::nplayer)
{
    ui->setupUi(this);
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
