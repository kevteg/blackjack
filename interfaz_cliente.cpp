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

interfaz_cliente::~interfaz_cliente()
{
    delete ui;
}
