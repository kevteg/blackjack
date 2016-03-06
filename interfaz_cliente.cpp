#include "interfaz_cliente.h"
#include "ui_interfaz_cliente.h"

interfaz_cliente::interfaz_cliente(QWidget *parent) : QWidget(parent), ui(new Ui::interfaz_cliente){
    ui->setupUi(this);
    ui->ir_atras->setIcon(QIcon(QPixmap(":/imágenes/Imágenes/back.png")));
    timer = new QTimer(this);
    connect(ui->ir_atras, SIGNAL(clicked(bool)), this, SLOT(atras()));
    connect(timer, SIGNAL(timeout()), this, SLOT(resetServerList()));
    ui->titulosw->setVisible(false);
    timer->start(5000);
}


void interfaz_cliente::atras(){
    emit goInitInterface();
}

void interfaz_cliente::addinListServer(QString server_name, QString server_ip, int time, int space){
    bool add = true;
    for(QVector <serverItem*>::iterator servidor = servidores.begin(); servidor != servidores.end() && add; servidor++){
        if((*servidor)->getData().contains(server_ip) && (*servidor)->getData().contains(server_name))
            add = false;
        if(!add){
            (*servidor)->updateTime(time);
            (*servidor)->updateSpace(space);
        }
    }
    if(add){
        servidores.append(new serverItem(server_name, server_ip, time, space));
        ui->serversLayout->addWidget(servidores.back());
        connect(servidores.back(), SIGNAL(seleccionServidor(QString)), this, SLOT(verifyConnectedServer(QString)));
        if(!ui->titulosw->isVisible())
            ui->titulosw->setVisible(true);
    }
}
void interfaz_cliente::resetServerList(){
    if(servidores.count()){
        for(QVector <serverItem*>::iterator servidor = servidores.begin(); servidor != servidores.end(); servidor++){
            (*servidor)->setVisible(false);
            ui->serversLayout->removeWidget(*servidor);
        }
        ui->titulosw->setVisible(false);
        servidores.clear();
    }
}

void interfaz_cliente::verifyConnectedServer(QString datos_servidor){
    emit conexionTcpCliente(datos_servidor);
}

interfaz_cliente::~interfaz_cliente()
{
    delete ui;
}
