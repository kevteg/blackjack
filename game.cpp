#include "game.h"
//Si es servidor sera el ultimo en el vector si es cliente el primero
game::game(int tipo_juego, QObject *parent) : QObject(parent), tipo_juego(tipo_juego){
    this->panel = NULL;
    this->jugadores = NULL;
}

void game::setPanel(panel_juego *panel){
    this->panel = panel;
}

void game::setJugadores(QVector<nplayer*> *jugadores){
    this->jugadores = jugadores;
    for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end() && panel; jug++){
        if(*jug == *this->jugadores->begin() && tipo_juego == protocolo::cliente)
            panel->addPlayer(true, (*jug));
        else if(*jug == this->jugadores->back() && tipo_juego == protocolo::servidor)
            panel->addPlayer(true, (*jug));
        else
            panel->addPlayer(false, (*jug));
    }
}


void game::beginGame(){

}
