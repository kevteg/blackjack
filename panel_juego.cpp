#include "panel_juego.h"
#include "ui_panel_juego.h"

panel_juego::panel_juego(QWidget *parent) : QWidget(parent), ui(new Ui::panel_juego){
    ui->setupUi(this);
    ui->ir_atras->setIcon(QIcon(QPixmap(":/imágenes/Imágenes/back.png")));
    connect(ui->ir_atras, SIGNAL(clicked(bool)), this, SLOT(goBack()));
}

panel_juego::~panel_juego()
{
    delete ui;
}

void panel_juego::goBack(){
    emit returnToInit();
}

