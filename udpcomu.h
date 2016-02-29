#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <QString>
#include <QThread>
#include <QUdpSocket>
#include <QMutex>
#define port 7777
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
            /**
             * @brief Se pone la dirección multicast que se usará para hablar o escribir
             */
            void    setDirMulticast(QString dir_multicast);
            /**
             * @brief Retorna la dirección multicast
             */
            QString getDirMulticast();

            void detenerBroadcast();
            bool enviandoBroadcast();
            void setManualyBr(bool);
            void listenBroadcast();
            void stopListeningBroadcast();
        signals:
            void broadcast(QString);
            void incomingData(QString);
        public slots:
            /*Ready read y disconnected se usan cuando se escuchan a los servidores*/
            void readyRead();
            void disconnected();
            void enviaBroadcast(QString data);
        private:
            QString     dir_multicast;
            udpComu     *ins_hilo;
            QUdpSocket  *socket;
            QThread     *hilo_broadcast;
            bool        enviando_broadcast;
            QMutex      mutex;
    };
}
#endif // COMUNICATION_H
