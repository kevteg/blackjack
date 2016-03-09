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
//emit enviar presentación
    llenarBaraja();
}

void game::llenarBaraja(){
    QDirIterator it(":/imágenes/Imágenes/Baraja", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        carta *carta_nueva = new carta(it.next().split(":/imágenes/Imágenes/Baraja")[1].split(".png")[0]);
        baraja.append(carta);
    }
}

carta game::getRandomCard(){
    carta nueva_carta = baraja.at(qrand() % (baraja.count() + 1));
    cartas_usadas.append(nueva_carta);
    return nueva_carta;
}

carta game::getRandomUsedCard(){
    carta nueva_carta = cartas_usadas.at(qrand() % (cartas_usadas.count() + 1));
    return nueva_carta;
}


