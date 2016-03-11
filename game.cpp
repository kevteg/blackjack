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
        if((*jug) == (this->jugadores->back()) && tipo_juego == protocolo::servidor)
            panel->addPlayer(true, (*jug));
        else if((*jug) == (*this->jugadores->begin()) && tipo_juego == protocolo::cliente)
            panel->addPlayer(true, (*jug));
        else
            panel->addPlayer(false, (*jug));
    }
}

void game::beginGame(){
//emit enviar comienzo de ronda

    prestamo = true;
    llenarBaraja();
    beginRound();
}

void game::beginRound(){
    if(baraja.count() < 10){
        //emit fin de juego

    }else{

        if(baraja.count() >= 10 && baraja.count() < 25){
            prestamo = false;
        }
        //emit comienzo de ronda
        //Cartas iniciales

        /* for(int i=0; i<2; i++){
            for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end() && panel; jug++){
                qDebug()<<"Turno de: "<<(*jug)->getId();
                if((*jug)==jugadores->back() && i==1)
                    break;
                carta carta_nueva=getRandomCard();
                //Enviar la carta
            }
        }
        //Pedir cartas extra
        for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end() && panel; jug++){
            if((*jug)!= jugadores->back()){
                while(true){
                    //enviar oferta de carte
                    //esperar respuesta de si quiere carta o no
                }
            }
        }*/

}
}

void game::llenarBaraja(){
    QDirIterator it(":/imágenes/Imágenes/Baraja", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        carta carta_nueva(it.next().split(":/imágenes/Imágenes/Baraja")[1].split(".png")[0]);
        baraja.append(carta_nueva);
        qDebug() << baraja.back().getNombre() << " card created";
   }
}

carta game::getRandomCard(){
    carta carta_nueva = baraja.at(qrand() % (baraja.count() + 1));
    cartas_usadas.append(carta_nueva);
    return carta_nueva;
}

carta game::getRandomUsedCard(){
    carta carta_nueva = baraja.at(qrand() % (baraja.count() + 1));
    return carta_nueva;
}

void game::bonification(int id){
    //Luego de recibir la señal de bonificacion
   if(round_count){
       for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end() && panel; jug++){
           if((*jug)->getId()==id && ((*jug)->getPuntos())>0)
               (*jug)->setBonificacion(true);
       }
   }
   else
       qDebug() << "Sin bonificación";
}
void game::enviarRonda(){
    QVector<QVariant> vector;
    for (QVector <nplayer*>::iterator jug = jugadores->begin(); jug != jugadores->end(); jug++) {
        vector.append((*jug)->getId());
        vector.append((*jug)->getPuntos());
    }
    sendMulticast(protocolo::cod_comienzo_ronda, vector);
}
