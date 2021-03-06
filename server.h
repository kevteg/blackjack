#ifndef SERVER_H
#define SERVER_H

/**
  Autores:
  Estefany Salas
  Kevin Hernández
  */

#include <QTcpServer>
#include "client_thread.h"
namespace Network
{
    class server : public QTcpServer
    {
        Q_OBJECT
    public:
        explicit server(QObject *parent = 0);
        bool startServer(QHostAddress adds, int port);
        void     stopServer();
        void     sendToClient(int socket_descriptor, QByteArray datos);
    signals:
        void messageFromClient(int, QString);
        void sendInfo(int socket_descriptor, QByteArray datos);
        void killAllConnections();
    protected:
       /**
         * @brief Manejar las conexiones entrantes. Este método se sobreescribió de QTcpServer
         * eso significa que mientras exista este objeto de servidor las conexiones entrantes
         * por el puerto y host se escucharan por el método sobreescrito abajo :)
         * @param s
         */
       void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
    private slots:
        void clientOut(int socket_des);
        void sendMessageFromClient(int, QString);
    signals:
        void clientSocketId(int);
        void clientOutofServer(int);
    private:
        int port;
        QHostAddress adds;
    };
}

#endif // SERVER_H
