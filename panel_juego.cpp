#include "panel_juego.h"
#include "ui_panel_juego.h"

panel_juego::panel_juego(QWidget *parent) : QWidget(parent), ui(new Ui::panel_juego){
    ui->setupUi(this);
    ui->ir_atras->setIcon(QIcon(QPixmap(":/imágenes/Imágenes/back.png")));
    connect(ui->ir_atras, SIGNAL(clicked(bool)), this, SLOT(goBack()));
    ui->groupBox->setStyleSheet("border: none");

}

panel_juego::~panel_juego()
{
    delete ui;
}
void panel_juego::dropPlayers(){
   /* QLayoutItem *child;
    while ((child = ui->local->takeAt(0)) != 0)  {
        delete child;
    }
    while ((child = ui->externo->takeAt(0)) != 0)  {
        delete child;
    }*/
}

void panel_juego::addPlayer(bool isPlayer, nplayer *new_player){
    if(isPlayer)
        ui->local->addWidget(new_player);
    else
        ui->externo->addWidget(new_player);
}

void panel_juego::goBack(){
    emit returnToInit();
}

