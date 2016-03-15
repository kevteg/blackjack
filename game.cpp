#include "game.h"
//Si es servidor sera el ultimo en el vector si es cliente el primero
game::game(int tipo_juego, QObject *parent) : QObject(parent), tipo_juego(tipo_juego){
    this->panel = NULL;
    this->jugadores = NULL;
    vertimer = new QTimer(this);
    connect(vertimer, SIGNAL(timeout()), this, SLOT(verifyStatus()));
    round_count = 0;
    ignore = 0;
}

void game::setPanel(panel_juego *panel){
    this->panel = panel;
}

void game::setJugadores(QVector<nplayer*> *jugadores){
    this->jugadores = jugadores;
    for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end() && panel; jug++){
        if((*jug) == (this->jugadores->back()) && tipo_juego == protocolo::servidor)
            panel->addPlayer(true, (*jug));
        else if((jug) == (this->jugadores->begin()) && tipo_juego == protocolo::cliente)
            panel->addPlayer(true, (*jug));
        else
            panel->addPlayer(false, (*jug));
    }
}

void game::beginGame(){
//emit enviar comienzo de ronda
    llenarBaraja();
    if(tipo_juego == protocolo::servidor){
        prestamo = round_count?true:false;
        status  = protocolo::comienzo_ronda;
        qDebug() << "Begin game";
        vertimer->start(protocolo::tiempo_inicio_ronda);
        beginner_player = jugadores->begin();
        //Luego de que se inicia el timer con 30 segundos se inicia la ronda
    }else{

    }
}

void game::beginRound(){
    if(baraja.count() < 10){
        //emit fin de juego
    }else{
        qDebug() << "count: " << baraja.count();
        if(baraja.count() < 25)
            prestamo = false;

        enviarRonda();
        turn_player = beginner_player;
    }
}
void game::verifyStatus(){
    switch (status) {
    case protocolo::comienzo_ronda:{
        status = protocolo::ronda;
        round_count = 0;
        qDebug() << "Time is up";
        beginRound();
        vertimer->stop();
        vertimer->start(protocolo::tiempo_espera_carta);
        break;
    }case protocolo::ronda:{ //PONER SEGUNDA RONDA DE REPARTIR CARTAS
        if(!round_count){
            if((*turn_player) != jugadores->back())
                sendCardToTurnPlayer();
            else if(!(*turn_player)->getCardsCount())
                 sendCardToTurnPlayer();

            if((*turn_player) != jugadores->back()){
                if((*turn_player)->getCardsCount())
                    turn_player++;
            }else{
                if((*turn_player)->getCardsCount() && (*jugadores->begin())->getCardsCount() == 2)
                  round_count++;
                turn_player = beginner_player;
            }
        }else{
            //Ignore se usa por si un cliente no responde la solicitud después de 2 veces
            //Verificar los turnos
            //Verificar el prestamo
            if(ignore == 2){
                if((*turn_player) != jugadores->back())
                    turn_player++;
                else
                    turn_player = jugadores->begin();
                ignore = 0;
            }
            if((*turn_player)->getCartasSum() >= 21){
                if((*turn_player) != jugadores->back())
                    turn_player++;
                else
                    turn_player = jugadores->begin();
                ignore = 0;
            }

            QVector<QVariant> data;
            if((*turn_player) != jugadores->back()){
                if((*turn_player)->getCartasSum() < 21 && jugadores->back()->getCartasSum() < 17){
                    data.append((*turn_player)->getId());
                    emit sendUnicast(tipo_juego, protocolo::cod_ofrecer_carta, data, ((*turn_player)->getSocketDes()));
                }else
                    renewRound();
            }else{
                if(jugadores->back()->getCartasSum() < 17)
                    sendCardToTurnPlayer();
                else
                    renewRound();

                turn_player = jugadores->begin();
                ignore = 0;
            }
            ignore++;
        }
        panel->changeBarajaValue(baraja.count());
        qDebug() << "Baraja: " << baraja.count();
        break;
    }
    default:
        break;
    }
}
 void game::renewRound(){
     bool all_zero = true;
     for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end(); jug++)
        if((*jug)->sumUpPoints())
            all_zero = false;
    if(all_zero){
        int high = 0;
        for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end(); jug++)
            if((*jug)->getCartasSum() < 21 && (*jug)->getCartasSum() > high)
                high = (*jug)->getCartasSum();

        for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end(); jug++)
            if((*jug)->getCartasSum() == high)
                (*jug)->setPuntos(1);

    }
    for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end(); jug++){
        qDebug() << "Sumatoria de cartas: " << (*jug)->getCartasSum() << "Puntos: " << (*jug)->getPuntos();
    }
    for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end(); jug++)
        (*jug)->resetCards();
     status = protocolo::comienzo_ronda;
     vertimer->stop();
     vertimer->start(protocolo::tiempo_inicio_ronda);
 }

void game::llenarBaraja(){
    QDirIterator it(":/imágenes/Imágenes/Baraja", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString nom = it.next().split(":/imágenes/Imágenes/Baraja")[1].split(".png")[0].split("/")[1];
        baraja.append(carta(nom));
        baraja.append(carta(nom));
        qDebug() << baraja.back().getNombre() << " card created";
   }
}

carta game::getRandomCard(){
    QTime now = QTime::currentTime();
    qsrand(now.msec());
    int pos = qrand() % (baraja.count());
    carta carta_nueva = baraja.at(pos);
    cartas_usadas.append(carta_nueva);
    baraja.removeAt(pos);
    return carta_nueva;
}

carta game::getRandomUsedCard(){
    carta carta_nueva = baraja.at(qrand() % (baraja.count() + 1));
    return carta_nueva;
}
void game::cardOffering(){
    //AQUI es donde va la estrategia del cliente. Cuando le ofrecen carta al cliente este es el método que lo recibe
    //Se debe ver si se pedira bono y eso
    //La estrategia a continuación es por motivos de prueba
    QVector<QVariant> data;
    if((*this->jugadores->begin())->getCartasSum() < 17){
        data.append(true);
    }else{
        data.append(false);
    }
    emit sendUnicast(tipo_juego, protocolo::cod_respuesta_carta, data);
}

void game::cardReply(int socket_des, bool resp){
    //AQUI el servidor recibe la respuesta del cliente, si quiere o no carta
    ignore = 0;
    if(socket_des == (*turn_player)->getSocketDes()){
        if(resp){
            sendCardToTurnPlayer();
        }else
            if((*turn_player) != jugadores->back())
                turn_player++;
            else
                turn_player = jugadores->begin();
    }
}

void game::sendCardToTurnPlayer(){
    QVector<QVariant> data;
    data.append((*turn_player)->getId());
    carta new_card = getRandomCard();
    data.append(new_card.getNombre());
    emit sendMulticast(protocolo::cod_envio_carta, data);
    (*turn_player)->addCard(new_card);
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

//Método que recibe la carta que envia el servidor
void game::cardInfo(int id, carta card){
    for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end() && panel; jug++){
        if(id == (*jug)->getId()){
            (*jug)->addCard(card);
            takeOffCard(card);
        }
    }
    panel->changeBarajaValue(baraja.count());
    /* Si la carta es para mi se puede hacer al mas
    if(id == (jugadores->begin())->getId())
        receiveCard(card);*/
}

void game::setPlayersPoints(QVector<int> *ids, QVector<int> *points){
    int i = 0;
    if(ids->count() && points->count())
    for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end(); jug++){
        if(i < ids->count() && i < points->count())
            if((*jug)->getId() == ids->at(i))
                (*jug)->setPuntosNoSum(points->at(i++));
        (*jug)->resetCards();
    }
}

//este método solo lo llaman los clientes para sacar cartas del mazo
void game::takeOffCard(carta ncarta){
    bool out = false;
    for(int var = 0; var < baraja.count() && !out; var++)
        if(baraja[var].getNombre() == ncarta.getNombre()){
            cartas_usadas.append(baraja.at(var));
            baraja.removeAt(var);
        }
}
void game::stopGame(){
    vertimer->stop();
}

void game::enviarRonda(){
    QVector<QVariant> vector;
    for (QVector <nplayer*>::iterator jug = jugadores->begin(); jug != jugadores->end(); jug++) {
        vector.append((*jug)->getId());
        vector.append((*jug)->getPuntos());
    }
    emit sendMulticast(protocolo::cod_comienzo_ronda, vector);
}
