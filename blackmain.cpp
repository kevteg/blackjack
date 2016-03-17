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
    ngame     = NULL;
    ui->gridLayout->addWidget(inter_ini);
    panel_principal = new panel_juego();
    mySelf = new nplayer;
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
    connect(com_udp, SIGNAL(incomingMulticastData(QString)), this, SLOT(multicastData(QString)));
    connect(panel_principal, SIGNAL(returnToInit()), this, SLOT(goInitInterface()));
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
            game_as = protocolo::cliente;
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
    if(ngame)
        ngame->stopGame();
    if(inter_ser){
        inter_ser->setVisible(false);
        servidor->stopServer();
        conteo_server   = 0;
        conteo_clientes = 0;
        if(timer->isActive())
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
    if(panel_principal->isVisible()){
        panel_principal->setVisible(false);
        panel_principal = new panel_juego;
        connect(panel_principal, SIGNAL(returnToInit()), this, SLOT(goInitInterface()));
        dropAllPlayers();
    }

}

void blackmain::serverSelected(){
    if(inter_ini->getBarra()->toPlainText() != ""){
        qDebug() << "Blackjack will run as server";
        inter_ini->setVisible(false);
        game_as = protocolo::servidor;
        current_state = protocolo::waiting_clients;
        conteo_clientes = 0;
        conteo_server = 0;
        if(!inter_ser){
            inter_ser = new interfaz_servidor(this);
            servidor = new Network::server;
            connect(inter_ser, SIGNAL(goInitInterface()), this, SLOT(goInitInterface()));
            connect(servidor, SIGNAL(messageFromClient(int, QString)), this, SLOT(tcpMessagesFromCLient(int, QString)));
            /*connect(servidor, SIGNAL(clientSocketId(int)), this, SLOT(setSocketIdToClient(int)));*/
            connect(servidor, SIGNAL(clientOutofServer(int)), this, SLOT(takeDisconnectedClientOut(int)));
            connect(inter_ser, SIGNAL(GameStart()), this, SLOT(loadGameInterface()));
        }else
            inter_ser->setVisible(true);
        if(!servidor->startServer(QHostAddress(local_ip), protocolo::tcp_port)){
            windowMessage("Error creando servidor", "¿Hay otro funcionando?");
            game_as = protocolo::nothing;
            current_state = protocolo::nothing;
            goInitInterface();
        }else{
            ui->gridLayout->addWidget(inter_ser);
            timer->start(1000);
        }
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
    //Minimo dos
    ui->statusBar->showMessage(tr("Conectandose al servidor seleccionado"));
    if(cliente->connectToHost(dir_ip, protocolo::tcp_port)){
        QVector <QVariant> var;
        var.append(inter_ini->getNombreUsuario());
        cliente->write(protocolo::generateJson(protocolo::cod_solicitud, &var));
        //qDebug() << "HERE: " << protocolo::generateJson(protocolo::cod_solicitud, &var);
        current_state = protocolo::waiting_server_res;
    }else
        clientSelected();
}

void blackmain::windowMessage(QString tittle, QString message){
    QMessageBox::about(this, tittle, message);
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
        timer->stop();
        sendPresentation();
    }else
        open = false;

    if(open){
        ui->gridLayout->addWidget(panel_principal);
        panel_principal->setVisible(true);
    }else{
        current_state = protocolo::nothing;
        windowMessage("No se conecto nadie :'(", "No se conectó ningún cliente. \n ¿Probamos de nuevo?");
        ui->statusBar->showMessage(tr(":'("));
    }
}

void blackmain::countServerTime(){
    if(conteo_server >= protocolo::max_time ||  conteo_clientes >= protocolo::max_players){
        loadGameInterface();
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
    //Varios tipos de emit, para cartas y eso que manejara la clase del juego
    QVector<QVariant> *vector_datos = protocolo::JsonToVector(data.toUtf8());
    if(vector_datos){
        switch (vector_datos->at(0).toInt()) {
        case protocolo::cod_solicitud:
            if(current_state == protocolo::waiting_clients){
                jugadores.append(new nplayer());
                (jugadores.back())->setName(vector_datos->at(1).toString());
                (jugadores.back())->setId(++conteo_clientes);
                (jugadores.back())->setSocketDes(socket_des);
                QVector <QVariant> respuesta;
                respuesta.append(true);
                respuesta.append(dir_multicast);
                respuesta.append(conteo_clientes);
                servidor->sendToClient(socket_des, protocolo::generateJson(protocolo::cod_aceptacion, &respuesta));
                inter_ser->addClientToList(&(*jugadores.back()));
            }else{
                QVector <QVariant> respuesta;
                respuesta.append(false);
                respuesta.append("null");
                respuesta.append(0);
                servidor->sendToClient(socket_des, protocolo::generateJson(protocolo::cod_aceptacion, &respuesta));
            }
            break;
        case protocolo::cod_respuesta_carta:
            if(current_state == protocolo::playing)
                ngame->cardReply(socket_des, vector_datos->at(1).toBool());
            break;
        default:
            break;
        }

    }else{
        ui->statusBar->showMessage(tr("Error  con la data"));
        qDebug() << "Fatal error with client in socket " << socket_des << ". Data: " << data;
    }
}
void blackmain::takeDisconnectedClientOut(int socket_des){
    bool out = false;
    for(QVector <nplayer*>::iterator s_player = jugadores.begin(); s_player != jugadores.end() && !out; s_player++)
        if((*s_player)->getSocketDes() == socket_des){
                inter_ser->outCLientFromList((*s_player));
            if(current_state == protocolo::playing){
                QVector<QVariant> var;
                var.append((*s_player)->getId());
                com_udp->sendMulticastMessage(protocolo::generateJson(protocolo::cod_error, &var));
                (*s_player)->playerGone();
            }
            conteo_clientes--;
            if(jugadores.removeOne((*s_player)))
                out = true;
        }
    //Enviar a los otros clientes mensaje de error
    if(!conteo_clientes && game_as == protocolo::servidor){
        goInitInterface();
        windowMessage("Se fueron todos los clientes :'(", "¿Probamos de nuevo?");
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
                mySelf->setId(vector_datos->at(3).toInt());
                mySelf->setName(inter_ini->getBarra()->toPlainText());
                loadGameInterface();
                com_udp->joinMulticast(direccion_multicast_ser);
                //unir mySelf a la lista de vector
                jugadores.append(mySelf);
            }else{
                goInitInterface();
                ui->statusBar->showMessage(tr("No fuimos aceptados en el juego :'("));
            }

        }
        break;
    case protocolo::cod_ofrecer_carta:{
        if(current_state == protocolo::playing){
            ngame->cardOffering();
        }
    }
        break;
    default:
        break;
    }
}
void blackmain::multicastData(QString data){
    qDebug() << "Incoming data from multicast"<< current_state<<": " << data;
    QVector<QVariant> *vec_datos = protocolo::JsonToVector(data.toUtf8());
    switch (vec_datos->at(0).toInt()) {
        case protocolo::cod_presentacion:
            if(game_as == protocolo::cliente){
                int i = 0;
                bool save = false;
                int id;
                QString name;
                //El último es el servidor
                for(QVector<QVariant>::iterator var = vec_datos->begin(); var != vec_datos->end(); var++){
                    if(i == 1)
                        name = var->toString();
                    else if(i == 2){
                        id = var->toInt();
                        save = true;
                    }
                    if(save)
                        if(id != mySelf->getId()){
                            jugadores.append(new nplayer());
                            (jugadores.back())->setName(name);
                            (jugadores.back())->setId(id);
                        }
                    save = false;
                    i = (!i || i == 1)?(i + 1):(1);
                }
                ngame = new game(game_as);
                connect(ngame, SIGNAL(sendMulticast(int,QVector<QVariant>)), this, SLOT(sendMulticast(int,QVector<QVariant>)));
                connect(ngame, SIGNAL(sendUnicast(int,int,QVector<QVariant>, int)), this, SLOT(sendUnicast(int,int, QVector<QVariant>, int)));
                ngame->setPanel(panel_principal);
                ngame->setJugadores(&jugadores);
                current_state = protocolo::playing;
                ngame->beginGame();
            }

            break;
        case protocolo::cod_comienzo_ronda:
            if(current_state == protocolo::playing && game_as == protocolo::cliente){
                int i = 0;
                QVector<int> ids;
                QVector<int> points;
                for(QVector<QVariant>::iterator var = vec_datos->begin(); var != vec_datos->end(); var++){
                    if(i)
                        if(!(i % 2))
                            points.append(var->toInt());
                        else
                            ids.append(var->toInt());
                    i++;
                }
                ngame->setPlayersPoints(&ids, &points);
            }
            break;
        case protocolo::cod_envio_carta:
            if(game_as == protocolo::cliente){
                int id = vec_datos->at(1).toInt();
                QString carta_id = vec_datos->at(2).toString();
                ngame->cardInfo(id, carta(carta_id));
            }
            break;
        case protocolo::cod_final_juego:{
            if(current_state == protocolo::playing && game_as == protocolo::cliente){
                int i = 0;
                int rounds = vec_datos->at(1).toString().toInt();
                int cards  = vec_datos->at(2).toString().toInt();
                bool empate = vec_datos->at(3).toString() == "true"?true:false;

                QVector<int> ids;
                QVector<int> points;
                for(int var = 4; var < vec_datos->count(); var++){
                    qDebug() << vec_datos->at(var).toInt();
                    if(i)
                        points.append(vec_datos->at(var).toInt());
                    else
                        ids.append(vec_datos->at(var).toInt());
                    i = !i?1:0;
                }

                ngame->finishClientGame(cards, rounds, empate, &ids, &points);
            }
        }
            break;
        case protocolo::cod_error:
            if(game_as == protocolo::cliente && current_state == protocolo::playing){
                bool out = false;
                 for(QVector <nplayer*>::iterator jug = jugadores.begin(); jug != jugadores.end() && !out; jug++)
                     if(vec_datos->at(1).toInt() == (*jug)->getId()){
                         (*jug)->playerGone();
                         out = true;
                     }
            }
            break;
        default:
            break;
    }

}

void blackmain::sendPresentation(){
    com_udp->joinMulticast(dir_multicast);
    //Agregar a myself a los jugadores
    mySelf->setId(conteo_clientes + 1);
    mySelf->setName(inter_ini->getBarra()->toPlainText());
    jugadores.append(mySelf);
    QVector <QVariant> respuesta;
    for (QVector <nplayer*>::iterator jug = jugadores.begin(); jug != jugadores.end(); jug++) {
        respuesta.append((*jug)->getName());
        respuesta.append((*jug)->getId());
    }
    com_udp->sendMulticastMessage(protocolo::generateJson(protocolo::cod_presentacion, &respuesta));
    ngame = new game(game_as);
    connect(ngame, SIGNAL(sendMulticast(int,QVector<QVariant>)), this, SLOT(sendMulticast(int,QVector<QVariant>)));
    connect(ngame, SIGNAL(sendUnicast(int,int,QVector<QVariant>, int)), this, SLOT(sendUnicast(int,int, QVector<QVariant>, int)));
    connect(ngame, SIGNAL(goInit()), this, SLOT(goInitInterface()));
    ngame->setPanel(panel_principal);
    ngame->setJugadores(&jugadores);
    ngame->beginGame();
    //Despues enviar el comienzo de ronda
}

void blackmain::sendMulticast(int cod, QVector<QVariant> vector){
    com_udp->sendMulticastMessage(protocolo::generateJson(cod, &vector));
}

void blackmain::sendUnicast(int tipo, int cod, QVector<QVariant> vector, int socket_des){
    if(tipo == protocolo::cliente)
        cliente->write(protocolo::generateJson(cod, &vector));
    else
        servidor->sendToClient(socket_des, protocolo::generateJson(cod, &vector));
}

void blackmain::dropAllPlayers(){
    jugadores.clear();
}

blackmain::~blackmain(){
    delete ui;
}

