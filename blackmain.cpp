#include "blackmain.h"
#include "ui_blackmain.h"

blackmain::blackmain(QWidget *parent) : QMainWindow(parent),ui(new Ui::blackmain){
    ui->setupUi(this);
    this->setWindowTitle("Blackjack");
    this->setFixedSize(this->size());
    //En este layout va la imagen y el edit para el nombre
    QVBoxLayout *layout_1 = new QVBoxLayout;
    //En este layout van los botones iniciales de selección
    QHBoxLayout *layout_2 = new QHBoxLayout;
    opciones_iniciales = new QToolButton*[n_opc_ini];
    barra_username     = new QTextEdit("Tu nombre");
    barra_username->setFixedSize(200, 25);
    imagen_inicio      = new image(QPixmap(":/imágenes/Imágenes/bj.jpg"), 200, 200);

    layout_1->setAlignment(Qt::AlignCenter);
    layout_1->addWidget(imagen_inicio);
    layout_1->addWidget(barra_username);

    for (int indice = 0; indice < n_opc_ini; indice++){
        opciones_iniciales[indice] = new QToolButton();
        opciones_iniciales[indice]->setText(!indice?"SERVIDOR":"CLIENTE");
        layout_2->addWidget(opciones_iniciales[indice]);
    }

    /*CSS*/
    this->setStyleSheet("QMainWindow{\
                            background: rgb(39, 174, 96); \
                         }\
                        QToolButton{\
                            background: rgb(231, 76, 60);\
                            border-radius: 3px;\
                            font: 15px; \
                            padding: 6px;\
                        }\
                        QToolButton:hover { \
                            background-color: rgb(192, 57, 43); \
                        }\
                        QToolButton:!hover { \
                            background-color: rgb(231, 76, 60); \
                        }\
                        ");
    this->ui->ventana_inicial->setStyleSheet("QGroupBox{\
                                                background: rgb(39, 174, 96);\
                                                border: none;\
                                             }");

    ui->layout_ven_inicial->addLayout(layout_1);
    ui->layout_ven_inicial->addLayout(layout_2);
    setConnections();
}
void blackmain::setConnections(){
    connect(opciones_iniciales[client], SIGNAL(clicked(bool)), this, SLOT(clientSelected()));
    connect(opciones_iniciales[server], SIGNAL(clicked(bool)), this, SLOT(serverSelected()));
    connect(ui->actionCr_ditos, SIGNAL(triggered(bool)), this, SLOT(about()));
}
void blackmain::clientSelected(){
    qDebug() << "Se ha seleccionado cliente";
}
void blackmain::serverSelected(){
    qDebug() << "Se ha seleccionado servidor";
}
void blackmain::about(){
    QMessageBox::about(this, tr("Acerca de Blackjack"),
                           tr("Programado por Estefany Salas y Kevin Hernández"
                              "\nUniversidad Nacional Experimental del Táchira - Comunicaciones I"));
}

blackmain::~blackmain()
{
    delete ui;
}
