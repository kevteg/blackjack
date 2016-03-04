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
                        QProgressBar { \
                            border: 0px;\
                            border-radius: 1px;\
                            text-align: center;\
                        }\
                        QProgressBar::chunk {\
                            background-color: rgb(41, 128, 185);\
                        }\
                        ");
    com_udp = new Network::udpComu();
    connect(com_udp, SIGNAL(incomingData(QString, QString)), this, SLOT(processUdpData(QString, QString)));
    connect(panel_principal, SIGNAL(returnToInit()), this, SLOT(goInitInterface()));
    conteo_server   = 0;
    conteo_clientes = 0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(countServerTime()));
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
        conteo_server   = 0;
        conteo_clientes = 0;
        timer->stop();
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
        //QJsonDocument trama(writeServerInfo());
        //com_udp->startBroadcast(trama.toJson());
        timer->start(1000);
    }else
        ui->statusBar->showMessage(tr("¡Selecciona tu nombre como sevidor!"));
}

QJsonObject blackmain::writeServerInfo(){
    /*Información para el broadcast*/
    QJsonObject info_s;
    info_s["id"]       = protocolo::cod_presentacion;
    info_s["nombre"]   = inter_ini->getNombreUsuario();
    info_s["tiempo"]   = conteo_server;
    info_s["espacios"] = conteo_clientes;
    return info_s;
}

void blackmain::about(){
    QMessageBox::about(this, tr("Acerca de Blackjack"),
                           tr("Programado por Estefany Salas y Kevin Hernández"
                              "\nUniversidad Nacional Experimental del Táchira - Comunicaciones I"));
}

void blackmain::processUdpData(QString sender_ip, QString data){
    QJsonDocument trama = QJsonDocument::fromJson(data.toUtf8());
    QStringList lista  = sender_ip.split(":");
    if(trama.object()["id"].toInt() == protocolo::cod_presentacion)
        inter_cli->addinListServer(trama.object()["nombre"].toString(), lista.value(lista.count() - 1), trama.object()["tiempo"].toInt(),  trama.object()["espacios"].toInt());
}
void blackmain::connectToTcpClient(QString dir_ip){
    com_udp->stopListeningBroadcast();
    qDebug() << "I am going to connect to " << dir_ip;
    ui->statusBar->showMessage(tr("Conectandose al servidor seleccionado"));
    cliente->connectToHost(dir_ip, tcp_port);
    loadGameInterface();
}
void blackmain::noClients(){
    QMessageBox::about(this, tr("No se conecto nadie :'("),
                           tr("No se conectó ningún cliente. \n ¿Probamos de nuevo?"));
}

void blackmain::loadGameInterface(){
    bool open = true;
    if(inter_ser->isVisible() && !conteo_clientes){
         qDebug() << "Time up: No clients connected :'(";
         goInitInterface();
         open = false;
    }else if(inter_cli->isVisible())
        inter_cli->setVisible(false);
    else if(inter_ser->isVisible())
        inter_ser->setVisible(false);
    else
        open = false;

    if(open){
        ui->gridLayout->addWidget(panel_principal);
        panel_principal->setVisible(true);
    }else
        noClients();
}

void blackmain::countServerTime(){
    if(conteo_server >= protocolo::max_time)
        loadGameInterface();
    else{
        conteo_server++;
        this->inter_ser->updateTime(conteo_server);
        QJsonDocument trama(writeServerInfo());
        com_udp->enviaUnicoBroadcast(trama.toJson());
    }

}

blackmain::~blackmain(){
    delete ui;
}
