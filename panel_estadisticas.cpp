#include "panel_estadisticas.h"
#include "ui_panel_estadisticas.h"

panel_estadisticas::panel_estadisticas(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::panel_estadisticas)
{
    ui->setupUi(this);
    color = protocolo::gray;
    this->setStyleSheet("QGroupBox{\
                         border: none;\
                         background: #34495e;}\
                         QLabel{color: white}");
    this->resize(this->width() + this->width()*1/2, this->height());
    connect(ui->toolButton, SIGNAL(clicked(bool)), this, SLOT(botonPressed()));
}

void panel_estadisticas::addItem(QString name, int points, int desempate){
    item_lista *it = new item_lista(name, points, desempate, this->color);
    this->ui->players_layout->addWidget(it);
    this->color = (this->color == protocolo::gray)?protocolo::white:protocolo::gray;
}

void panel_estadisticas::setData(int cartas, int rounds, QString desempate){
    this->ui->cartas->setText("Cartas usadas: " + QString::number(cartas));
    this->ui->rounds->setText("Rounds jugados: " + QString::number(rounds));
    this->ui->desempate->setText("Desempate: " + desempate);
    if(desempate == "No")
        this->ui->label_desempate->setVisible(false);
}

void panel_estadisticas::botonPressed(){
    emit goInit();
}

panel_estadisticas::~panel_estadisticas()
{
    delete ui;
}
