#include "server.h"

Network::server::server(QObject *parent) : QTcpServer(parent){

}
void Network::server::startServer(QHostAddress adds, int port){
    if(!this->listen(adds, port))
       qDebug() << "Server couldn't start. " << adds << " adress:" << port;
    else
       qDebug() << "Server started. Listening petitions at " << adds << ":" << port;
}

/*El socketdescriptor lo maneja el sistema operativo*/
void Network::server::incomingConnection(qintptr socketDescriptor){
      qDebug() << "Connecting to socket " << socketDescriptor;
      client_thread *my_conn = new client_thread(socketDescriptor);
      connect(this,    SIGNAL(killAllConnections()), my_conn, SLOT(closeConnection()));
      connect(my_conn, SIGNAL(finished()),   my_conn, SLOT(deleteLater()));
      my_conn->start();
}

void Network::server::stopServer(){
    emit killAllConnections();
}
