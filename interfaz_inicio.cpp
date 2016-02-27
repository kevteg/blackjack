#include "interfaz_inicio.h"
#include "ui_interfaz_inicio.h"

interfaz_inicio::interfaz_inicio(QWidget *parent) : QWidget(parent), ui(new Ui::interfaz_inicio){
    ui->setupUi(this);
    QVBoxLayout *contenedor = new QVBoxLayout;
    layout_1                = new QVBoxLayout;
    layout_2                = new QHBoxLayout;
    opciones_iniciales      = new QToolButton*[n_opc_ini];
    barra_username          = new QTextEdit();
    barra_username->setStatusTip("¿Qué nombre quieres tener durante la partida?");
    barra_username->setFixedSize(200, 25);
    imagen_inicio           = new image(QPixmap(":/imágenes/Imágenes/bj.jpg"), 200, 200);
    layout_1->setAlignment(Qt::AlignCenter);
    layout_1->addWidget(imagen_inicio);
    layout_1->addWidget(barra_username);
    for (int indice = 0; indice < n_opc_ini; indice++){
        opciones_iniciales[indice] = new QToolButton();
        opciones_iniciales[indice]->setText(!indice?"SERVIDOR":"CLIENTE");
        layout_2->addWidget(opciones_iniciales[indice]);
    }
    opciones_iniciales[SERVIDOR]->setStatusTip("Selecciona para jugar como casa");
    opciones_iniciales[CLIENTE]->setStatusTip("Selecciona para jugar como cliente");
    contenedor->addLayout(layout_1);
    contenedor->addLayout(layout_2);
    ui->gridLayout->addLayout(contenedor, ui->gridLayout->rowCount()/2, ui->gridLayout->columnCount()/2);
    connect(opciones_iniciales[SERVIDOR], SIGNAL(clicked(bool)), this, SLOT(emitServidor()));
    connect(opciones_iniciales[CLIENTE], SIGNAL(clicked(bool)), this, SLOT(emitCliente()));

}
void interfaz_inicio::emitServidor(){
    emit serverSelected();
}

void interfaz_inicio::emitCliente(){
    emit clientSelected();
}
QTextEdit* interfaz_inicio::getBarra(){
    return barra_username;
}

interfaz_inicio::~interfaz_inicio(){
    delete ui;
}
