#ifndef DATASEND_H
#define DATASEND_H

/**
  Autores:
  Estefany Salas
  Kevin Hernández
  */

#include <QTcpSocket>

namespace Network
{
    namespace Base
    {
        class DataSend
        {
            private:
                 QTcpSocket *socket;

            public:
                explicit DataSend(QTcpSocket *socket = NULL);
                //setters
                 /**
                 * @brief Este método se usa si no se definio el socket en el constructor
                 * @param socket
                 */
                void set_socket_DS(QTcpSocket *socket);
                /**
                 * @brief Escribir a través del socket el mensaje s
                 * @param s
                 */
                void write(QByteArray data);
        };
    }
}

#endif // DATASEND_H
