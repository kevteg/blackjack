#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "MultipurposeSocket_ReadyToUse/MultiPropuseSockets.h"

class communication
{
public:
    communication();
    /**
     * @brief inicializa el programa como servidor o cliente
     */
    void init(Network::Base::Data *object);
    /**
     * @brief Envia los mensajes broadcast
     */
    void spreadInfo();
    /**
     * @brief retorna al objeto que se usa para comunicarse, sea cliente o servidor
     * @return
     */
    Network::Base::Data *getComobj();
private:
//Este objeto puede ser un cliente o un servidor
    Network::Base::Data *com_object;

};

#endif // COMUNICATION_H
