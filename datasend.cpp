#include "datasend.h"

Network::Base::DataSend::DataSend(QTcpSocket *socket){
    this->socket = socket;
}
void Network::Base::DataSend::set_socket_DS(QTcpSocket *socket)
{
    this->socket = socket;
}
void Network::Base::DataSend::write(QString s)
{
    socket->write(s.toUtf8());
    socket->flush();
}
