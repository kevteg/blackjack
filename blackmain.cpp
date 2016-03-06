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
    current_state = protocolo::nothing;
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
                connect(cliente, SIGNAL(serverOut()), this, SLOT(goInitInterface()));
                connect(cliente, SIGNAL(dataReceived(QString)), this, SLOT(tcpMessagesFromServer(QString)));
            }else
                inter_cli->setVisible(true);
            ui->gridLayout->addWidget(inter_cli);
            com_udp->listenBroadcast();
        }else
            ui->statusBar->showMessage(tr("¡Selecciona tu nombre como cliente!"));

}

void blackmain::goInitInterface(){
    current_state = protocolo::nothing;
    inter_ini->setVisible(true);
    if(inter_ser){
        inter_ser->setVisible(false);
        servidor->stopServer();
        conteo_server   = 0;
        conteo_clientes = 0;
        timer->stop();
    }if(inter_cli){
        inter_cli->setVisible(false);
        if(cliente->isConnected())
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
        current_state = protocolo::waiting_clients;
        if(!inter_ser){
            inter_ser = new interfaz_servidor(this);
            servidor = new Network::server;
            servidor->startServer(QHostAddress(local_ip), tcp_port);
            connect(inter_ser, SIGNAL(goInitInterface()), this, SLOT(goInitInterface()));
            connect(servidor, SIGNAL(messageFromClient(int, QString)), this, SLOT(tcpMessagesFromCLient(int, QString)));
            /*connect(servidor, SIGNAL(clientSocketId(int)), this, SLOT(setSocketIdToClient(int)));*/
            connect(servidor, SIGNAL(clientOutofServer(int)), this, SLOT(takeDisconnectedClientOut(int)));
            connect(inter_ser, SIGNAL(GameStart()), this, SLOT(loadGameInterface()));
        }else
            inter_ser->setVisible(true);
        ui->gridLayout->addWidget(inter_ser);
        timer->start(1000);
    }else
        ui->statusBar->showMessage(tr("¡Selecciona tu nombre como sevidor!"));
}

void blackmain::about(){
    QMessageBox::about(this, tr("Acerca de Blackjack"),
                           tr("Programado por Estefany Salas y Kevin Hernández"
                              "\nUniversidad Nacional Experimental del Táchira - Comunicaciones I"));
}

void blackmain::processUdpData(QString sender_ip, QString data){
    QVector<QVariant> *vector_datos = protocolo::JsonToVector(data.toUtf8());
    QStringList lista  = sender_ip.split(":");
    /*También hacer una a través del protocolo*/
    if(vector_datos->at(0).toInt() == protocolo::cod_saludo)
        inter_cli->addinListServer(vector_datos->at(1).toString(), lista.value(lista.count() - 1), vector_datos->at(2).toInt(),  vector_datos->at(3).toInt());
}
void blackmain::connectToTcpClient(QString dir_ip){
    com_udp->stopListeningBroadcast();
    qDebug() << "I am going to connect to " << dir_ip;
    ui->statusBar->showMessage(tr("Conectandose al servidor seleccionado"));
    if(cliente->connectToHost(dir_ip, tcp_port)){
        QVector <QVariant> var;
        var.append(inter_ini->getNombreUsuario());
        cliente->write(protocolo::generateJson(protocolo::cod_solicitud, &var));
        current_state = protocolo::waiting_server_res;
    }else
        clientSelected();
}

void blackmain::noClients(){
    QMessageBox::about(this, tr("No se conecto nadie :'("),
                           tr("No se conectó ningún cliente. \n ¿Probamos de nuevo?"));
}

void blackmain::loadGameInterface(){
    bool open = true;

    if(inter_ser && inter_ser->isVisible() && !conteo_clientes){
         qDebug() << "Time up: No clients connected :'(";
         goInitInterface();
         open = false;
    }else if(inter_cli && inter_cli->isVisible()){
        inter_cli->setVisible(false);
        current_state = protocolo::waiting_game_to_start;
        ui->statusBar->showMessage(tr("¡Esperemos que lleguen los demás jugadores!"));
    }else if(inter_ser && inter_ser->isVisible()){
        inter_ser->setVisible(false);
        current_state = protocolo::playing;
        ui->statusBar->showMessage(tr("¡Comienza el juego!"));
    }else
        open = false;

    if(open){
        ui->gridLayout->addWidget(panel_principal);
        panel_principal->setVisible(true);

    }else{
        current_state = protocolo::nothing;
        noClients();
        ui->statusBar->showMessage(tr(":'("));
    }
}

void blackmain::countServerTime(){

    if(conteo_server >= protocolo::max_time ||  conteo_clientes >= protocolo::max_players){
        loadGameInterface();
        timer->stop();
    }else{
        conteo_server++;
        this->inter_ser->updateTime(conteo_server);
        QVector <QVariant> var;
        var.append(inter_ini->getNombreUsuario());
        var.append(conteo_server);
        var.append(conteo_clientes);
        com_udp->enviaUnicoBroadcast(protocolo::generateJson(protocolo::cod_saludo, &var));
    }
}

void blackmain::tcpMessagesFromCLient(int socket_des, QString data){
    //Aqui va todo lo que el cliente le dice al servidor por UNICAST
    QVector<QVariant> *vector_datos = protocolo::JsonToVector(data.toUtf8());
    switch (vector_datos->at(0).toInt()) {
    case protocolo::cod_solicitud:
        if(current_state == protocolo::waiting_clients){
            jugadores.append(new player());
            (jugadores.back())->setName(vector_datos->at(1).toString());
            (jugadores.back())->setId(++conteo_clientes);
            (jugadores.back())->setSocketDes(socket_des);
            QVector <QVariant> respuesta;
            respuesta.append(true);
            respuesta.append(dir_multicast);
            respuesta.append(conteo_clientes);
            servidor->sendToClient(socket_des, protocolo::generateJson(protocolo::cod_aceptacion, &respuesta));
            inter_ser->addClientToList(*jugadores.back());
        }
        break;
    default:
        break;
    }
}
void blackmain::takeDisconnectedClientOut(int socket_des){
    for(QVector <player*>::iterator s_player = jugadores.begin(); s_player != jugadores.end(); s_player++)
        if((*s_player)->getSocketDes() == socket_des){
            inter_ser->outCLientFromList((**s_player));
            conteo_clientes--;
        }
}

void blackmain::tcpMessagesFromServer(QString data){
    //Aqui va todo lo que el servidor le dice al cliente por UNICAST
    QVector<QVariant> *vector_datos = protocolo::JsonToVector(data.toUtf8());
    switch (vector_datos->at(0).toInt()) {
    case protocolo::cod_aceptacion:
        if(current_state == protocolo::waiting_server_res){ //If estado esperando respuesta
            if(vector_datos->at(1).toInt()){
                direccion_multicast_ser = vector_datos->at(2).toString();
                mySelf.setId(vector_datos->at(2).toInt());
                mySelf.setName(inter_ini->getBarra()->toPlainText());
                loadGameInterface();
            }else{
                goInitInterface();
                ui->statusBar->showMessage(tr("No fuimos aceptados en el juego :'("));
            }

        }
        break;
    default:
        break;
    }
}

blackmain::~blackmain(){
    delete ui;
}
