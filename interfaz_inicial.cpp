#include "interfaz_inicial.h"
#include "ui_interfaz_inicial.h"

interfaz_inicial::interfaz_inicial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::interfaz_inicial)
{
    ui->setupUi(this);
}

interfaz_inicial::~interfaz_inicial()
{
    delete ui;
}
