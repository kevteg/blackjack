#include "communication.h"

communication::communication(){

}
void communication::init(Network::Base::Data *object){
    this->com_object = object;
}
Network::Base::Data *communication::getComobj(){
    return com_object;
}
