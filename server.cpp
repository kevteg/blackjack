#include "server.h"

Network::server::server(QObject *parent) : QTcpServer(parent){

}

bool Network::server::startServer(QHostAddress adds, int port){
    bool started = true;
    if(!this->listen(adds, port)){
       qDebug() << "Server couldn't start. " << adds << " adress:" << port;
       started = false;
    }else
       qDebug() << "Server started. Listening petitions at " << adds << ":" << port;
    return started;
}

/*El socketdescriptor lo maneja el sistema operativo*/
void Network::server::incomingConnection(qintptr socketDescriptor){
      qDebug() << "Connecting to socket " << socketDescriptor;
      client_thread *my_conn = new client_thread(socketDescriptor);
      connect(this,    SIGNAL(killAllConnections()), my_conn, SLOT(closeConnection()));
      connect(my_conn, SIGNAL(finished()),   my_conn, SLOT(deleteLater()));
      connect(my_conn, SIGNAL(messageFromClient(int, QString)),   this, SLOT(sendMessageFromClient(int, QString)));
      connect(this, SIGNAL(sendInfo(int, QByteArray)), my_conn, SLOT(sendInformation(int, QByteArray)));
      connect(my_conn, SIGNAL(clientOut(int)), this, SLOT(clientOut(int)));
      my_conn->start();
}

void Network::server::stopServer(){
    if(this->isListening()){
        emit killAllConnections();
        this->close();
    }
}
void Network::server::clientOut(int socket_des){
    emit clientOutofServer(socket_des);
}

void Network::server::sendToClient(int socket_descriptor, QByteArray datos){
    emit sendInfo(socket_descriptor, datos);
}

void Network::server::sendMessageFromClient(int socket_d, QString data){
    emit messageFromClient(socket_d, data);
}
