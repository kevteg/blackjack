#include "client_thread.h"

Network::client_thread::client_thread(int s_id, QObject *parent) : QThread(parent){
    this->socket_des = s_id;
}

void Network::client_thread::run(){
    qDebug() << "Starting client socket: "<< socket_des <<" in thread";
    socket = new QTcpSocket();
    if(socket->setSocketDescriptor(this->socket_des)){
        this->set_socket_DS(socket);
        qDebug() << "Connected to socket: "<< socket_des;
        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);
    }else{
        qDebug() << "Can not connect to " << socket_des << " client :(";
        emit error(socket->error());
    }
    exec();
}

void Network::client_thread::disconnected(){
    qDebug() << "Disconnected from " << this->socket_des;
    emit clientOut(this->socket_des);
    socket->deleteLater();
    exit(0);
}

void Network::client_thread::closeConnection(){
    socket->deleteLater();
    exit(0);
}

void Network::client_thread::sendInformation(int socket_des, QByteArray datos){
    if(socket_des == this->socket_des)
        this->write(datos);
}

void Network::client_thread::readyRead(){
    QString s(socket->readAll());
    qDebug() << "Received from client " << socket->socketDescriptor() << ": " << s;
    emit messageFromClient(this->socket_des, s);
}

