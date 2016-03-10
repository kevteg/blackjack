#include "panel_juego.h"
#include "ui_panel_juego.h"

panel_juego::panel_juego(QWidget *parent) : QWidget(parent), ui(new Ui::panel_juego){
    ui->setupUi(this);
    ui->ir_atras->setIcon(QIcon(QPixmap(":/imágenes/Imágenes/back.png")));
    connect(ui->ir_atras, SIGNAL(clicked(bool)), this, SLOT(goBack()));
    ui->groupBox->setStyleSheet("border: none");
    players = 0;
}

panel_juego::~panel_juego()
{
    delete ui;
}

void panel_juego::addPlayer(bool isPlayer, nplayer *new_player){
    players++;
    if(isPlayer){
        new_player->isMe();
        ui->local->addWidget(new_player);
    }else{
        if(players > 3)
            ui->externo1->addWidget(new_player);
        else
            ui->externo2->addWidget(new_player);
    }

}

void panel_juego::goBack(){
    emit returnToInit();
}

