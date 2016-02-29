#include "interfaz_cliente.h"
#include "ui_interfaz_cliente.h"

interfaz_cliente::interfaz_cliente(QWidget *parent) : QWidget(parent), ui(new Ui::interfaz_cliente){
    ui->setupUi(this);
    ui->ir_atras->setIcon(QIcon(QPixmap(":/imágenes/Imágenes/back.png")));
    connect(ui->ir_atras, SIGNAL(clicked(bool)), this, SLOT(atras()));
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(limpiarListaServidores()));
    ui->conectar_servidor->setEnabled(false);
    connect(ui->lista_servidores, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemPresionado()));
    connect(ui->conectar_servidor, SIGNAL(clicked(bool)), this, SLOT(verifyConnectedServer()));
    timer->start(5000);
}

void interfaz_cliente::itemPresionado(){
    ui->conectar_servidor->setEnabled(true);
}

void interfaz_cliente::atras(){
    emit goInitInterface();
}

void interfaz_cliente::addinListServer(QString server_name, QString server_ip, int server_port){
    bool add = true;
    for(int i = 0; i < ui->lista_servidores->count() && add; i++){
        QListWidgetItem* item = ui->lista_servidores->item(i);
        if(item->text() == server_name + " (Ip: " + server_ip  + ":" + QString::number(server_port) + ")")
            add = false;
    }
    if(add){
        ui->lista_servidores->addItem(new QListWidgetItem(server_name + " (Ip: " + server_ip + ":" + QString::number(server_port) + ")"));
        servidores << server_name + separador_string + server_ip;
    }
}
void interfaz_cliente::verifyConnectedServer(){
     QStringList ser_info = servidores.at(ui->lista_servidores->row(ui->lista_servidores->currentItem())).split(separador_string);
     emit conexionTcpCliente(ser_info.value(ser_info.count() - 1));
}

void interfaz_cliente::limpiarListaServidores(){
    ui->lista_servidores->clear();
    servidores.clear();
    ui->conectar_servidor->setEnabled(false);
}

interfaz_cliente::~interfaz_cliente()
{
    delete ui;
}
