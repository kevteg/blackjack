#include "client.h"

Network::Client::Client(QString host, int port, QObject *parent): QObject(parent), Network::Base::DataSend(&socket),
    host(host),
    port(port)
{
    connected_B = false;
}

void Network::Client::connectToHost(){
    connect(&socket, SIGNAL(connected()), this, SLOT(connected())); //Necesario para mostrar las alertas al conectarse
    socket.connectToHost(host, port);
    connected_B = socket.waitForConnected(3000);

    if(connected_B){//Ya se conecto aqui
        connect(&socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        connect(&socket, SIGNAL(readyRead()),    this, SLOT(readyRead()));
    }
    else{
        disconnect(&socket,SIGNAL(connected()),this,SLOT(connected())); //Si no se conecto se desconecta la señal
        qDebug()<<"Error connecting to: "<<host<<":"<<port;
    }
}

void Network::Client::connected(){
    qDebug()<<"Conected to " << host << ":" << port;
}

void Network::Client::disconnected(){
     qDebug()<<"Disconnected to " << host << ":" << port;
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
void Network::Client::readyRead(){
    QString s(socket.readAll());
    qDebug() << s << " Received from server";
    emit dataReceived(s);
}
