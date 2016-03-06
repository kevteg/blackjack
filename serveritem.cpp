#include "serveritem.h"
#include "ui_serveritem.h"

serverItem::serverItem(QString server_name, QString ip, int time, int space, QWidget *parent) :
    QWidget(parent),
    ip(ip),
    time(time),
    space(space),
    name(server_name),
    ui(new Ui::serverItem)
{
    ui->setupUi(this);
    ui->server_button->setText(this->name);
    ui->time_progress->setMaximum(protocolo::max_time);
    ui->time_progress->setValue(time);
    ui->space_progress->setMaximum(protocolo::max_players);
    ui->space_progress->setValue(space);
    this->setStyleSheet("QToolButton{width: 150px;}");
    connect(ui->server_button, SIGNAL(clicked(bool)), this, SLOT(sendServerData()));

}

void serverItem::updateTime(int time){
    this->time = time;
    ui->time_progress->setValue(time);
}

void serverItem::updateSpace(int space){
    this->space = space;
    ui->space_progress->setValue(space);
}

int serverItem::getTime(){
    return time;
}

QString serverItem::getIp(){
    return ip;
}

QString serverItem::getData(){
    return ip + " " + name;
}

void serverItem::sendServerData(){
    emit seleccionServidor(getIp());
}

serverItem::~serverItem()
{
    delete ui;
}
