#include "blackmain.h"
#include "ui_blackmain.h"

blackmain::blackmain(QWidget *parent) : QMainWindow(parent),ui(new Ui::blackmain){
    ui->setupUi(this);
    this->setWindowTitle("Blackjack");
    inter_ini = new interfaz_inicio(this);
    inter_cli = NULL;
    inter_ser = NULL;
    cliente   = NULL;
    servidor  = NULL;
    ui->gridLayout->addWidget(inter_ini);
    panel_principal = new panel_juego();
    /*Encontrar la ip del equipo*/
    QList<QHostAddress> dir = QNetworkInterface::allAddresses();
    foreach (QHostAddress item, dir)
        if(item.protocol() == QAbstractSocket::IPv4Protocol && item != QHostAddress::LocalHost && item != QHostAddress::LocalHostIPv6)
            local_ip = item.toString();

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
    connect(panel_principal, SIGNAL(returnToInit()), this, SLOT(goInitInterface()));
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
                cliente = new Network::Client();
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
    if(inter_ser){
        inter_ser->setVisible(false);
        servidor->stopServer();
    }if(inter_cli){
        inter_cli->setVisible(false);
        cliente->closeConnection();
    }
    if(com_udp->enviandoBroadcast())
        com_udp->detenerBroadcast();
    else if(cliente && cliente->isConnected())
        com_udp->stopListeningBroadcast();
    if(panel_principal->isVisible())
        panel_principal->setVisible(false);
}

void blackmain::serverSelected(){
    if(inter_ini->getBarra()->toPlainText() != ""){
        qDebug() << "Blackjack will run as server";
        inter_ini->setVisible(false);
        if(!inter_ser){
            inter_ser = new interfaz_servidor(this);
            servidor = new Network::server;
            servidor->startServer(QHostAddress(local_ip), tcp_port);
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
    com_udp->stopListeningBroadcast();
    qDebug() << "I am going to connect to " << dir_ip;
    ui->statusBar->showMessage(tr("Conectandose al servidor seleccionado"));
    cliente->connectToHost(dir_ip, tcp_port);
    inter_cli->setVisible(false);
    ui->gridLayout->addWidget(panel_principal);
    panel_principal->setVisible(true);
}

blackmain::~blackmain(){
    delete ui;
}
