#include "udpcomu.h"

Network::udpComu::udpComu(){
    enviando_broadcast = false;
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
     qDebug() << "Listening to servers broadcast in port: " << port;
     socket = new QUdpSocket(this);
     connect(socket, SIGNAL(readyRead()),    this, SLOT(readyRead()));
     connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
     socket->bind(port);
}
void Network::udpComu::stopListeningBroadcast(){
    socket->deleteLater();
}

void Network::udpComu::enviaBroadcast(QString datos){
    socket = new QUdpSocket(this);
    connect(socket, SIGNAL(readyRead()),    this, SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    socket->connectToHost(QHostAddress::Broadcast, port);
    while(enviando_broadcast){
        QByteArray data;
        data.append(datos);
        qDebug() << "Sending: " << datos;
        QThread::msleep(100);
        socket->writeDatagram(data, QHostAddress::Broadcast, port);
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
    emit incomingData(data);
}
void Network::udpComu::setManualyBr(bool var){
    enviando_broadcast = var;
}

void Network::udpComu::disconnected(){
    qDebug() << "Udp socket Disconnected";
    socket->deleteLater();
    exit(0);
}


void Network::udpComu::setDirMulticast(QString dir_multicast){
    this->dir_multicast = dir_multicast;
}
bool Network::udpComu::enviandoBroadcast(){
    return enviando_broadcast;
}

QString Network::udpComu::getDirMulticast(){
    return this->dir_multicast;
}
