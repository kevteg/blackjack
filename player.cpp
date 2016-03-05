#include "player.h"

player::player()
{
    puntos=0;
}
QString player::getName(){
    return name;
}
void player::setName(QString name){
    this->name= name;
}
void player::setId(int id){
    this->id= id;
}
int player::getId(){
    return id;
}
void player::setSocketDes(int socket_descriptor){
    this->socket_descriptor= socket_descriptor;
}
int player::getSocketDes(){
    return socket_descriptor;
}
