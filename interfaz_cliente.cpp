#include "interfaz_cliente.h"
#include "ui_interfaz_cliente.h"

interfaz_cliente::interfaz_cliente(QWidget *parent) : QWidget(parent), ui(new Ui::interfaz_cliente){
    ui->setupUi(this);
    ui->ir_atras->setIcon(QIcon(QPixmap(":/imágenes/Imágenes/back.png")));
    connect(ui->ir_atras, SIGNAL(clicked(bool)), this, SLOT(atras()));
}


void interfaz_cliente::atras(){
    emit goInitInterface();
}

void interfaz_cliente::addinListServer(QString server_name, QString server_ip, int time, int space){
    bool add = true;

    for(QVector <serverItem*>::iterator servidor = servidores.begin(); servidor != servidores.end() && add; servidor++){
        if((*servidor)->getData().contains(server_ip))
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
    }
}
void interfaz_cliente::verifyConnectedServer(QString datos_servidor){
    emit conexionTcpCliente(datos_servidor);
}

interfaz_cliente::~interfaz_cliente()
{
    delete ui;
}
