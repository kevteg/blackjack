#include "client.h"

Network::Client::Client(QObject *parent): QObject(parent), Network::Base::DataSend(socket){
    connected_B = false;
}

bool Network::Client::connectToHost(QString host, int port){
    this->host = host;
    this->port = port;
    socket = new QTcpSocket();
    connect(socket, SIGNAL(connected()), this, SLOT(connected())); //Necesario para mostrar las alertas al conectarse
    socket->connectToHost(host, port);
    connected_B = socket->waitForConnected(3000);

    if(connected_B){//Ya se conecto aqui
        this->set_socket_DS(socket);
        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        connect(socket, SIGNAL(readyRead()),    this, SLOT(readyRead()));
    }
    else{
        disconnect(socket,SIGNAL(connected()),this,SLOT(connected())); //Si no se conecto se desconecta la señal
        qDebug()<<"Error connecting to: "<<host<<":"<<port;
    }
    return connected_B;
}

void Network::Client::connected(){
    qDebug() << "Conected to " << host << ":" << port;
}

void Network::Client::disconnected(){
     qDebug()<<"Disconnected to " << host << ":" << port;
     emit serverOut();
}

bool Network::Client::isConnected()   {
    return connected_B;
}

QString Network::Client::getHost(){
    return host;
}

int Network::Client::getPort(){
    return port;
}

void Network::Client::closeConnection(){
    socket->disconnectFromHost();
}

void Network::Client::readyRead(){
    QString s(socket->readAll());
    qDebug() << s << " Received from server";
    emit dataReceived(s);
}
