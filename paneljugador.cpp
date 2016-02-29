#include "paneljugador.h"
#include "ui_paneljugador.h"

panelJugador::panelJugador(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::panelJugador)
{
    ui->setupUi(this);
}

panelJugador::~panelJugador()
{
    delete ui;
}
