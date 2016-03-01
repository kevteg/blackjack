#ifndef CLIENT_H
#define CLIENT_H
/**
  Autores:
  Estefany Salas
  Kevin Hernández
  */

#include "datasend.h"
namespace Network
{
    class Client : public QObject, public Base::DataSend
    {
        Q_OBJECT
    public:
        explicit Client(QObject *parent = NULL);
     /**
         * @brief Conectar al host que llego en el constructor con el puerto indicado
         */
        bool connectToHost(QString host, int port);
        /**
         * @brief isConnected
         * @return true si ya esta conectado, false lo contrario
         */
        bool    isConnected();
        /**
         * @brief getHost
         * @return el host al que esta conectado
         */
        QString getHost();
        /**
         * @brief getPort
         * @return retorna el puerto por el que esta conectado al host
         */
        int     getPort();

     signals:
        /**
          * @brief Esta señal informa al juego que cosa llega desde el servidor
          * @param data
          */
         void dataReceived(QString data);
     private slots:
         /*Todos estos slots deben avisar a través de signals lo que esta pasando con el servidor*/
         /**
         * @brief Este slot se activa cuando hay información disponible para el cliente
         */
        void readyRead();
        /**
        * @brief Este slot se activa cuando se realiza la conexión
        */
        void	connected();
        /**
        * @brief Este slot se activa cuando se pierde la conexión
        */
        void	disconnected();
        //void	bytesWritten(qint64 bytes);
 public:
        void closeConnection();
 private:
        QTcpSocket *socket;
        QString     host;
        int         port;
        bool        connected_B;
 };
}

#endif // CLIENT_H

