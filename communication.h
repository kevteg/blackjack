#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <QString>
//#include "MultipurposeSocket_ReadyToUse/MultiPropuseSockets.h"

class communication
{
    public:
        communication();
        /**
         * @brief Envia los mensajes broadcast
         */
        void    spreadInfo(QString *datos);
        void    stopInfoSpeading();
        void    setDirMulticast(QString dir_multicast);
        QString getDirMulticast();

    private:
        QString dir_multicast;
        bool spreading;
};

#endif // COMUNICATION_H
