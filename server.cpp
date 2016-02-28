#include "server.h"

Network::server::server(QHostAddress adds, int port, QObject *parent) : QTcpServer(parent),
    adds(adds),
    port(port)
{}
void Network::server::startServer(){
    if(!this->listen(adds, port))
         qDebug() << "Server couldn't start. " << adds << " adress:" << port;
    else
       qDebug() << "Server started. Listening petitions at " << adds << ":" << port;
}
/*El socketdescriptor lo maneja el sistema operativo*/
void Network::server::incomingConnection(qintptr socketDescriptor){
      qDebug() << "Connecting to socket " << socketDescriptor;
      client_thread *my_conn = new client_thread(socketDescriptor);
      connect(my_conn, SIGNAL(finished()), my_conn, SLOT(deleteLater()));
      my_conn->start();
}
