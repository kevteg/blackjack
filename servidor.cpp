#include "servidor.h"
#include "ui_servidor.h"

servidor::servidor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::servidor)
{
    ui->setupUi(this);
}

servidor::~servidor()
{
    delete ui;
}
