#include "panel_estadisticas.h"
#include "ui_panel_estadisticas.h"

panel_estadisticas::panel_estadisticas(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::panel_estadisticas)
{
    ui->setupUi(this);
}
void panel_estadisticas::setItems(QVector<nplayer *> *jugadores, int cartas, int rounds){
    int i = protocolo::gray;
    for(QVector <nplayer*>::iterator jug = jugadores->begin(); jug != jugadores->end(); jug++){
        items.append(new item_lista((*jug)->getName(), (*jug)->getPuntos(), 0, i));
        this->ui->players_layout->addWidget(items.back());
        i = !i?protocolo::gray:protocolo::white;
    }
    this->ui->cartas->setText("Cartas usadas: " + cartas);
    this->ui->cartas->setText("Rounds usadas: " + rounds);
}

panel_estadisticas::~panel_estadisticas()
{
    delete ui;
}
