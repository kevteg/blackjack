#include "panel_estadisticas.h"
#include "ui_panel_estadisticas.h"

panel_estadisticas::panel_estadisticas(int cartas, int rounds, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::panel_estadisticas)
{
    ui->setupUi(this);
    color = protocolo::gray;
    this->ui->cartas->setText("Cartas usadas: " + cartas);
    this->ui->rounds->setText("Rounds: " + rounds);
    this->setStyleSheet("QGroupBox{\
                         border: none;\
                         background: #34495e;}\
                         QLabel{color: white}");
    connect(ui->toolButton, SIGNAL(clicked(bool)), this, SLOT(botonPressed()));
}

void panel_estadisticas::addItem(QString name, int points){
    item_lista *it = new item_lista(name, points, 0, this->color);
    this->ui->players_layout->addWidget(it);
    this->color = (this->color == protocolo::gray)?protocolo::white:protocolo::gray;
}
void panel_estadisticas::botonPressed(){
    emit goInit();
}

panel_estadisticas::~panel_estadisticas()
{
    delete ui;
}
