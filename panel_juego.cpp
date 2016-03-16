#include "panel_juego.h"
#include "ui_panel_juego.h"

panel_juego::panel_juego(QWidget *parent) : QWidget(parent), ui(new Ui::panel_juego){
    ui->setupUi(this);
    ui->ir_atras->setIcon(QIcon(QPixmap(":/imágenes/Imágenes/back.png")));
    connect(ui->ir_atras, SIGNAL(clicked(bool)), this, SLOT(goBack()));
    ui->groupBox->setStyleSheet("border: none");
    players = 0;
    rounds  = 0;
}

panel_juego::~panel_juego()
{
    delete ui;
}

void panel_juego::addPlayer(bool isPlayer, nplayer *new_player){
    if(isPlayer){
        new_player->isMe();
        ui->local->addWidget(new_player);
    }else{
        players++;
        if(players > 2)
            ui->externo1->addWidget(new_player);
        else
            ui->externo2->addWidget(new_player);
    }

}

void panel_juego::changeBarajaValue(int bar){
    ui->barajalabel->setText("Baraja: " + QString::number(bar));
}

void panel_juego::changeRondaValue(){
    rounds++;
    ui->roundlabel->setText("Rounds: " + QString::number(rounds));
}

int panel_juego::getRondaValue(){
    return rounds;
}

void panel_juego::goBack(){
    emit returnToInit();
}

