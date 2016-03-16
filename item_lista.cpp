#include "item_lista.h"
#include "ui_item_lista.h"

item_lista::item_lista(QString name, int points, int desempate, int color, QWidget *parent) : QWidget(parent), ui(new Ui::item_lista){
    ui->fondo->setStyleSheet("QGroupBox:border{none} QLabel{color: #34495e;}");
    ui->setupUi(this);
    set(name, points, desempate, color);
}
void item_lista::set(QString name, int points, int desempate, int color){
    if(color == protocolo::gray)
        ui->fondo->setStyleSheet("QGroupBox{\
                                  background: #bdc3c7;}");
    else
          ui->fondo->setStyleSheet("QGroupBox{\
                                    background: #ecf0f1;}");
    this->ui->name->setText(name);
    this->ui->points->setText(QString::number(points));
    this->ui->desempate->setText(QString::number(desempate));
}

item_lista::~item_lista()
{
    delete ui;
}
