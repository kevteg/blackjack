#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <QString>
#include <QThread>
#include <QUdpSocket>
#include <QMutex>
#include <QTimer>
#include "protocolo.h"
/**
  Autores:
  Estefany Salas
  Kevin Hernández
  */
namespace Network{
    class udpComu : public QObject
    {
        Q_OBJECT
        public:
            explicit udpComu();
            /**
             * @brief Crea un hilo de esta misma clase que envia los mensajes broadcast
             */
            void    startBroadcast(QString datos);

            void detenerBroadcast();
            bool enviandoBroadcast();
            void setManualyBr(bool);
            void listenBroadcast();
            void stopListeningBroadcast();
            void joinMulticast(QString dir_multicast);
            void sendMulticastMessage(QByteArray data);
        signals:
            void broadcast(QString);
            void incomingData(QString, QString);
            void incomingMulticastData(QString);
        public slots:
            /*Ready read y disconnected se usan cuando se escuchan a los servidores*/
            void readyRead();
            void disconnected();
            void readyReadMulticast();
            void enviaUnicoBroadcast(QString data);
            void enviaMultipleBroadcast(QString data); //Con este método de aqui se crea un hilo que se encargue

        private:
            udpComu     *ins_hilo;
            QUdpSocket  *socket;
/*            QUdpSocket  *sender_multi_socket;
            QUdpSocket  *receiver_multi_socket;*/
            QUdpSocket  *multi_socket;
            QThread     *hilo_broadcast;
            QHostAddress dir_multicast;
            bool         enviando_broadcast;
            bool         listening_broadcast;
    };
}
#endif // COMUNICATION_H
