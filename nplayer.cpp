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
                         background: #2ecc71;}\
                         QLabel{color: white}");
    puntos = 0;
    x = 20;
    y = 20;
    me = false;
    id = 0;
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
                         background: #BDBDBD;\
                         }");
}
void nplayer::isMe(){
    this->setStyleSheet("QGroupBox{\
                            background: #239a55;}\
                            QLabel{color: white}");
    me = true;
}

bool nplayer::getBonificacion(){
    return bonificacion;
}

void nplayer::setBonificacion(bool bonificacion){
    this->bonificacion= bonificacion;
}

void nplayer::addCard(carta new_card){
    new_card.getImage()->setParent(ui->widget);
    new_card.getImage()->move(x, y);
    new_card.getImage()->show();
    cartas.append(new_card);
    x += 20;
    y += me?20:0;
}
int  nplayer::getCardsCount(){
    return cartas.count();
}
int  nplayer::getCartasSum(){
    int sum = 0;
    int naz = 0;
    for(QVector <carta>::iterator card = this->cartas.begin(); card != this->cartas.end(); card++){
        if(card->getValue() < 0)
            naz++;
        else
            sum += card->getValue();
    }
    for (int var = 0; var < naz; var++)
        sum += (sum + 11 <= 21)?11:1;
    return sum;
}

int  nplayer::getPuntos(){
    return puntos;
}
void nplayer::setPuntos(int puntos){
    this->puntos=puntos;
}
