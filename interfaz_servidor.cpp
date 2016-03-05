#include "interfaz_servidor.h"
#include "ui_interfaz_servidor.h"

interfaz_servidor::interfaz_servidor(QWidget *parent) : QWidget(parent), ui(new Ui::interfaz_servidor){
    ui->setupUi(this);
    ui->progressBar->setMaximum(protocolo::max_time);
    ui->progressBar->setValue(0);
    ui->ir_atras->setIcon(QIcon(QPixmap(":/imágenes/Imágenes/back.png")));
    ui->comenzar_partida->setEnabled(false);
   // ui->clientes_conectados->set
    connect(ui->ir_atras, SIGNAL(clicked(bool)), this, SLOT(atras()));
}

void interfaz_servidor::atras(){
    emit goInitInterface();
}

void interfaz_servidor::updateTime(int time){
    this->time = time;
    ui->progressBar->setValue(time);
}

int interfaz_servidor::getTime(){
    return time;
}

interfaz_servidor::~interfaz_servidor(){
    delete ui;
}

void interfaz_servidor::addClientToList(player new_gamer){
    ui->clientes_conectados->addItem(new QListWidgetItem(new_gamer.getName()));
}
