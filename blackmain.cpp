#include "blackmain.h"
#include "ui_blackmain.h"

blackmain::blackmain(QWidget *parent) : QMainWindow(parent),ui(new Ui::blackmain){
    ui->setupUi(this);
    this->setWindowTitle("Blackjack");
    inter_ini = new interfaz_inicio(this);
    inter_cli = NULL;
    inter_ser = NULL;
    cliente   = NULL;
    ui->gridLayout->addWidget(inter_ini);
    /*QSS*/
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
                            color: white; \
                        }\
                        QToolButton:!hover { \
                            background-color: rgb(231, 76, 60); \
                            color: white; \
                        }\
                        QToolButton:!enabled { \
                            background-color: rgb(96,125,139); \
                            color: white; \
                        }\
                        QLabel{\
                               color: rgb(55,71,79);\
                               }\
                        QListWidget::item{\
                                          color:#2c3e50;\
                                          background-color:transparent;\
                                          ;}\
                        ");
    com_udp = new Network::udpComu();
    connect(com_udp, SIGNAL(incomingData(QString, QString, int)), this, SLOT(processUdpData(QString, QString, int)));
    setConnections();
}
void blackmain::setConnections(){
    connect(inter_ini, SIGNAL(serverSelected()), this, SLOT(serverSelected()));
    connect(inter_ini, SIGNAL(clientSelected()), this, SLOT(clientSelected()));
    connect(ui->actionCr_ditos, SIGNAL(triggered(bool)), this, SLOT(about()));
}
void blackmain::clientSelected(){
    if(inter_ini->getBarra()->toPlainText() != ""){
            qDebug() << "Blackjack will run as client";
            inter_ini->setVisible(false);
            if(!inter_cli){
                inter_cli = new interfaz_cliente(this);
                connect(inter_cli, SIGNAL(goInitInterface()),           this, SLOT(goInitInterface()));
                connect(inter_cli, SIGNAL(conexionTcpCliente(QString)), this, SLOT(connectToTcpClient(QString)));
            }else
                inter_cli->setVisible(true);
            ui->gridLayout->addWidget(inter_cli);
            com_udp->listenBroadcast();
        }else
            ui->statusBar->showMessage(tr("¡Selecciona tu nombre como cliente!"));

}
void blackmain::goInitInterface(){
    inter_ini->setVisible(true);
    if(inter_ser)
        inter_ser->setVisible(false);
    if(inter_cli)
        inter_cli->setVisible(false);
    if(com_udp->enviandoBroadcast())
        com_udp->detenerBroadcast();
    else
        com_udp->stopListeningBroadcast();
}

void blackmain::serverSelected(){
    if(inter_ini->getBarra()->toPlainText() != ""){
        qDebug() << "Blackjack will run as server";
        inter_ini->setVisible(false);
        if(!inter_ser){
            inter_ser = new interfaz_servidor(this);
            connect(inter_ser, SIGNAL(goInitInterface()), this, SLOT(goInitInterface()));
        }else
            inter_ser->setVisible(true);
        ui->gridLayout->addWidget(inter_ser);
        com_udp->startBroadcast("P:123.123.123.123:" + inter_ini->getNombreUsuario());
    }else
        ui->statusBar->showMessage(tr("¡Selecciona tu nombre como sevidor!"));
}
void blackmain::about(){
    QMessageBox::about(this, tr("Acerca de Blackjack"),
                           tr("Programado por Estefany Salas y Kevin Hernández"
                              "\nUniversidad Nacional Experimental del Táchira - Comunicaciones I"));
}
void blackmain::processUdpData(QString sender_ip, QString data, int puerto){
    qDebug() << data;
    QStringList pieces = data.split(":");
    QStringList lista  = sender_ip.split(":");
    if(pieces.value(0) == "P")
        inter_cli->addinListServer(pieces.value(2), lista.value(lista.count() - 1), puerto);
}
void blackmain::connectToTcpClient(QString dir_ip){
    qDebug() << "I am going to connect to " << dir_ip;
    //cliente = new Client
}

blackmain::~blackmain()
{
    delete ui;
}
