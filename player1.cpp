#include "player1.h"
#include "ui_paneljugador.h"

player::player(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::panelJugador)
{
    ui->setupUi(this);
}




player::~player()
{
    delete ui;
}
