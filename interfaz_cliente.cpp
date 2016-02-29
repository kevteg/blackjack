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
void interfaz_cliente::addinListServer(QString new_server){
    bool add = true;
    for(int i = 0; i < ui->lista_servidores->count() && add; i++){
        QListWidgetItem* item = ui->lista_servidores->item(i);
        if(item->text() == new_server)
            add = false;
    }
    if(add)
        ui->lista_servidores->addItem(new QListWidgetItem(new_server));
}

interfaz_cliente::~interfaz_cliente()
{
    delete ui;
}
