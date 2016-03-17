#include "item_lista.h"
#include "ui_item_lista.h"

item_lista::item_lista(QString name, int points, int desempate, int color, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::item_lista)
{
    this->setStyleSheet("QGroupBox{border: none;\
                                   height: 30px;}\
                             QLabel{color: #34495e;}");
    ui->setupUi(this);
    this->name = name;
    this->points = points;
    this->desempate = desempate;
    this->color = color;
    set();
}
void item_lista::set(){
    if(this->color == protocolo::gray)
        ui->fondo->setStyleSheet("QGroupBox{\
                                  background: #bdc3c7;}");
    else
          ui->fondo->setStyleSheet("QGroupBox{\
                                    background: #ecf0f1;}");
    this->ui->name->setText(this->name);
    this->ui->points->setText(QString::number(this->points));
    this->ui->desempate->setText(QString::number(this->desempate));
}

item_lista::~item_lista()
{
    delete ui;
}
