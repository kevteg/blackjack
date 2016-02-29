#include "panel_juego.h"
#include "ui_panel_juego.h"

panel_juego::panel_juego(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::panel_juego)
{
    ui->setupUi(this);
}

panel_juego::~panel_juego()
{
    delete ui;
}
