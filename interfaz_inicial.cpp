#include "interfaz_inicial.h"
#include "ui_interfaz_inicial.h"

interfaz_inicial::interfaz_inicial(QWidget *parent) : QWidget(parent), ui(new Ui::interfaz_inicial){
    ui->setupUi(this);
    imagen_inicial = new image(QPixmap(":/imágenes/Imágenes/bj.jpg"), 200, 200);
    ui->verticalLayout->addWidget(imagen_inicial);
    barra_username = new QTextEdit("Tu nombre");
    layout_buttons = new QHBoxLayout;
    for (int indice = 0; indice < n_opc_ini; indice++){
        opciones_iniciales[indice] = new QToolButton();
        opciones_iniciales[indice]->setText(!indice?"SERVIDOR":"CLIENTE");
        layout_buttons->addWidget(opciones_iniciales[indice]);
    }
    ui->verticalLayout->addWidget(barra_username);
    ui->verticalLayout->addLayout(layout_buttons);
}

interfaz_inicial::~interfaz_inicial(){
    delete ui;
}
