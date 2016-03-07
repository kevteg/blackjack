#include "udpcomu.h"

Network::udpComu::udpComu(){
    enviando_broadcast  = false;
    listening_broadcast = false;

}

void Network::udpComu::startBroadcast(QString datos){
    ins_hilo = new udpComu;
    hilo_broadcast = new QThread();
    ins_hilo->moveToThread(hilo_broadcast);
    connect(hilo_broadcast,  SIGNAL(finished()),         hilo_broadcast, SLOT(deleteLater()));
    connect(this,            SIGNAL(broadcast(QString)), ins_hilo,       SLOT(enviaBroadcast(QString)));
    ins_hilo->setManualyBr(true);
    this->enviando_broadcast = true;
    hilo_broadcast->start();
    emit broadcast(datos);
}

void Network::udpComu::listenBroadcast(){
    if(!listening_broadcast){
        socket = new QUdpSocket(this);
        connect(socket, SIGNAL(readyRead()),    this, SLOT(readyRead()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
         qDebug() << "Listening to servers broadcast in port: " << protocolo::udp_port;
         listening_broadcast = true;
         socket->bind(protocolo::udp_port);
    }
}

void Network::udpComu::stopListeningBroadcast(){
    if(listening_broadcast)
        socket->deleteLater();
    listening_broadcast = false;
}

void Network::udpComu::enviaUnicoBroadcast(QString datos){
    socket = new QUdpSocket(this);
    connect(socket, SIGNAL(readyRead()),    this, SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    socket->connectToHost(QHostAddress::Broadcast, protocolo::udp_port);
    QByteArray data;
    data.append(datos);
    qDebug() << "Sending: " << datos;
    socket->writeDatagram(data, QHostAddress::Broadcast, protocolo::udp_port);
}
void Network::udpComu::enviaMultipleBroadcast(QString datos){
    /*Para correr este broadcast es necesario correr el startbroadcast que crea un hilo para este metodo*/
    socket = new QUdpSocket(this);
    connect(socket, SIGNAL(readyRead()),    this, SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    socket->connectToHost(QHostAddress::Broadcast, protocolo::udp_port);
    while(enviando_broadcast){
        QByteArray data;
        data.append(datos);
        qDebug() << "Sending: " << datos;
        socket->writeDatagram(data, QHostAddress::Broadcast, protocolo::udp_port);
    }
}

void Network::udpComu::detenerBroadcast(){
    if(ins_hilo && ins_hilo->enviandoBroadcast()){
        this->enviando_broadcast = false;
        ins_hilo->setManualyBr(false);
        ins_hilo->deleteLater();
    }
}

void Network::udpComu::readyRead(){
    QByteArray data;
    data.resize(socket->pendingDatagramSize());
    QHostAddress sender;
    quint16 puerto;
    socket->readDatagram(data.data(), data.size(), &sender, &puerto);
    qDebug() << "Message ("<< sender.toString() << ":" << puerto <<"): " << data;
    emit incomingData(sender.toString(), data);
}

void Network::udpComu::setManualyBr(bool var){
    enviando_broadcast = var;
}

void Network::udpComu::disconnected(){
    qDebug() << "Udp socket Disconnected";
    socket->deleteLater();
    exit(0);
}

bool Network::udpComu::enviandoBroadcast(){
    return enviando_broadcast;
}
void Network::udpComu::readyReadMulticast(){
    QByteArray data;
    data.resize(multi_socket->pendingDatagramSize());
    multi_socket->readDatagram(data.data(), data.size());
    emit incomingMulticastData(QString(data));
}

void Network::udpComu::joinMulticast(QString dir_multicast){
    this->dir_multicast = QHostAddress(dir_multicast);
    multi_socket = new QUdpSocket;
    multi_socket->setSocketOption(QAbstractSocket::MulticastTtlOption, protocolo::udpTtl);
    multi_socket->bind(QHostAddress::AnyIPv4, protocolo::multi_port, QUdpSocket::ShareAddress);
    multi_socket->joinMulticastGroup(this->dir_multicast);
    connect(multi_socket, SIGNAL(readyRead()), this, SLOT(readyReadMulticast()));

}

void Network::udpComu::sendMulticastMessage(QByteArray data){
    multi_socket->writeDatagram(data, this->dir_multicast, protocolo::multi_port);
}
