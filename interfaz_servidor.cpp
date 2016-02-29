#include "interfaz_servidor.h"
#include "ui_interfaz_servidor.h"

interfaz_servidor::interfaz_servidor(QWidget *parent) : QWidget(parent), ui(new Ui::interfaz_servidor){
    ui->setupUi(this);
    ui->progressBar->setValue(ui->progressBar->minimum());
    ui->ir_atras->setIcon(QIcon(QPixmap(":/imágenes/Imágenes/back.png")));
    ui->comenzar_partida->setEnabled(false);
    connect(ui->ir_atras, SIGNAL(clicked(bool)), this, SLOT(atras()));
}

void interfaz_servidor::atras(){
    emit goInitInterface();
}

interfaz_servidor::~interfaz_servidor(){
    delete ui;
}
