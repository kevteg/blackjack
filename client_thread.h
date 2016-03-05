#ifndef CLIENT_THREAD_H
#define CLIENT_THREAD_H
/**
  Autores:
  Estefany Salas
  Kevin Hernández
  */

#include <QThread>
#include "datasend.h"
namespace Network
{
    class client_thread : public QThread, public Base::DataSend
    {
        Q_OBJECT
    public:
        explicit client_thread(int s_id, QObject *parent = 0);
        void     run();
    public slots:
        /**
         * @brief Slot que se activa cuando el socket de este cliente se desconecta
         */
        void disconnected();
        /**
         * @brief Cuando hay información disponible del cliente
         */
        void readyRead();
        void closeConnection();
        void sendInformation(int socket_des, QByteArray datos);
    signals:
        /**
         * @brief esto se emite al servidor si hay un error de algún tipo,
         * el servidor aun no lo implementa pero esta la posibilidad de usar
         * esta señal
         * @param serror
         */
        void error(QTcpSocket::SocketError serror);
        void messageFromClient(int, QString);
    private:
        /*Número que da el so al socket*/
        int         socket_des;
        QTcpSocket *socket;

    };
}

#endif // CLIENT_THREAD_H
