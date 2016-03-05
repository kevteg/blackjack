#include "datasend.h"

Network::Base::DataSend::DataSend(QTcpSocket *socket){
    this->socket = socket;
}
void Network::Base::DataSend::set_socket_DS(QTcpSocket *socket)
{
    this->socket = socket;
}
void Network::Base::DataSend::write(QByteArray data)
{
    if(socket->state() == QAbstractSocket::ConnectedState){
        socket->write(data);
        socket->flush();
    }
}
