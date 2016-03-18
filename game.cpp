#include "game.h"
//Si es servidor sera el ultimo en el vector si es cliente el primero
game::game(int tipo_juego, QObject *parent) : QObject(parent), tipo_juego(tipo_juego){
    this->panel = NULL;
    this->jugadores = NULL;
    vertimer = new QTimer(this);
    connect(vertimer, SIGNAL(timeout()), this, SLOT(verifyStatus()));
    round_count = 0;
    ignore = 0;
    panel_es = new panel_estadisticas();
    connect(panel_es, SIGNAL(goInit()), this, SLOT(goInitInterface()));
    panel_es->setVisible(false);
}

void game::setPanel(panel_juego *panel){
    this->panel = panel;
    panel_es->setParent(this->panel);
    panel_es->move(panel->width() / 2, 20);
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
    llenarBaraja();
    if(tipo_juego == protocolo::servidor){
        status  = protocolo::comienzo_ronda;
        qDebug() << "Begin game";
        vertimer->start(protocolo::tiempo_inicio_ronda);
        beginner_player = jugadores->begin();
    }else{

    }
}

bool game::beginRound(){
    bool retorno = true;
    if(baraja.count() < 90){
        finishGame();
        retorno = false;
    }else{
        qDebug() << "count: " << baraja.count();
        if(baraja.count() >= 10 && baraja.count() <= 25)
            prestamo = false;
        int ronda=panel->getRondaValue();
        qDebug() << "RONDA: "<< ronda;
        if(!ronda)
            beginner_player = jugadores->begin();
        else{
            while(ronda >= jugadores->count()){
                ronda -= (jugadores->count()-1);
            }
            for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end(); jug++){
                if((*jug)->getId()  ==  ronda+1)
                    beginner_player =   jug;
            }
            if(beginner_player  ==  &jugadores->back())
                beginner_player  =  jugadores->begin();
        }
        enviarRonda();
        turn_player = beginner_player;
    }

    return retorno;
}

void game::verifyStatus(){
    switch (status) {
    case protocolo::comienzo_ronda:{
        status = protocolo::ronda;
        round_count = 0;
        qDebug() << "Time is up";
        prestamo = true;
        vertimer->stop();
        if(beginRound()){
            vertimer->start(protocolo::tiempo_espera_carta);
            panel->changeRondaValue();
        }else{
            qDebug() << "Game over";
            panel_es->setData(cartas_usadas.count(), panel->getRondaValue(), false);
            for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end() && panel; jug++)
                panel_es->addItem((*jug)->getName(), (*jug)->getPuntos(), /*(*jug)->getDesempate()*/0);
            panel_es->setVisible(true);
            status = protocolo::fin;
        }
        break;
    }case protocolo::ronda:{
        if(!round_count){
            if((*turn_player) != jugadores->back() && (*turn_player)->getCardsCount()<2)
                sendCardToTurnPlayer();
            else if(!(*turn_player)->getCardsCount() && (*turn_player)->getCardsCount()<2)
                 sendCardToTurnPlayer();

            if((*turn_player) != jugadores->back()){
                if((*turn_player)->getCardsCount())
                    turn_player++;
            }else{
                if((*turn_player)->getCardsCount() && (*jugadores->begin())->getCardsCount() == 2 && (*turn_player)->getCartasSum() <= 21){
                    if(!prestamo)
                        sendCardToTurnPlayer();
                    round_count++;
                }
                turn_player = jugadores->begin();  //ESTE NO ES
            }
        }else if(prestamo){
            //Ignore se usa por si un cliente no responde la solicitud después de 2 veces
            if(ignore == 2){
                if((*turn_player) != jugadores->back())
                    turn_player++;
                else
                    turn_player = jugadores->begin();
                ignore = 0;
            }
            if((*turn_player)->getCartasSum() >= 21 || (!prestamo && (*turn_player)->getCardsCount() == 2)){
                if((*turn_player) != jugadores->back())
                    turn_player++;
                else{
                    turn_player = jugadores->begin();
                }
                ignore = 0;
            }

            QVector<QVariant> data;
            if((*turn_player) != jugadores->back()){
                if((*turn_player)->getCartasSum() < 21 && jugadores->back()->getCartasSum() < 17){

                    data.append((*turn_player)->getId());
                    emit sendUnicast(tipo_juego, protocolo::cod_ofrecer_carta, data, ((*turn_player)->getSocketDes()));

                }else{
                    if((jugadores->back()->getCartasSum() >= 17)  &&  (*jugadores->begin())->getCartasSum() >= 17 )
                         renewRound();
                }
            }else{
                if(jugadores->back()->getCartasSum() < 17 && (*jugadores->begin())->getCartasSum() >= 17)
                    sendCardToTurnPlayer();
                else{
                    if((*jugadores->begin())->getCartasSum() >= 17)
                         renewRound();
                }
                turn_player = jugadores->begin();
                ignore = 0;
            }
            ignore++;
        }else if(!prestamo && (*jugadores->begin())->getCardsCount() ==  2 && (jugadores)->back()->getCardsCount() == 2){
                renewRound();
        }

        panel->changeBarajaValue(baraja.count());
        //qDebug() << "Baraja: " << baraja.count();
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
            else if((*jug)->getCartasSum()<21 && (*jug)->getBonificacion())
                 (*jug)->setPuntos(-1);

    }
    for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end(); jug++){
        qDebug() << "Sumatoria de cartas: " << (*jug)->getCartasSum() << "Puntos: " << (*jug)->getPuntos();
    }
    for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end(); jug++){
        (*jug)->resetCards();
        (*jug)->setBonificacion(false);
    }
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
   if(protocolo::comienzo_ronda && tipo_juego == protocolo::cliente){
        //data.append(*this->jugadores->getId());
        data.append(true);
        emit sendUnicast(tipo_juego, protocolo::cod_bono, data);
    }
    if((*this->jugadores->begin())->getCartasSum() < 17){
        data.append(true);
    }else{
        data.append(false);
    }
    emit sendUnicast(tipo_juego, protocolo::cod_respuesta_carta, data);
}

void game::cardReply(int socket_des, bool resp){
    //AQUI el servidor recibe la respuesta del cliente, si quiere o n1  o carta
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

void game::bonification(int socketDes){
   if(panel->getRondaValue()){
       for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end() && panel; jug++){
           if((*jug)->getSocketDes()==socketDes && ((*jug)->getPuntos())>0)
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
    panel->changeRondaValue();
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
    for(QVector <nplayer*>::iterator jug = jugadores->begin(); jug != jugadores->end(); jug++) {
        vector.append((*jug)->getId());
        vector.append((*jug)->getPuntos());
    }
    emit sendMulticast(protocolo::cod_comienzo_ronda, vector);
}
void game::finishGame(){
    QVector<QVariant> vector;
    desempateFinal();
    vector.append(panel->getRondaValue());
    vector.append(cartas_usadas.count());
    for(QVector <nplayer*>::iterator jug = jugadores->begin(); jug != jugadores->end(); jug++) {
        qDebug() << "Id: " << (*jug)->getId() << ": " << (*jug)->getPuntos();
        vector.append((*jug)->getId());
        vector.append((*jug)->getPuntos());
    }
    vector.append(false);
    emit sendMulticast(protocolo::cod_final_juego, vector);

}

void game::desempateFinal(){
    int high = 0;
    QVector<nplayer*> des;
    for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end(); jug++)
        if((*jug)->getPuntos() > high)
            high = (*jug)->getPuntos();
    qDebug()<<"Mayor: "<<high;
    carta c_desempate = getRandomUsedCard();

    int aux=0;
    for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end(); jug++)
        if((*jug)->getPuntos() == high){
            aux++;
        }
    qDebug()<<"Aux: "<<aux;
    if(aux>1){
        for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end(); jug++){
            if((*jug)->getPuntos() == high){
                c_desempate=getRandomUsedCard();
                if(c_desempate.getValue() == -1)
                    (*jug)->setDesempate(11);
                else
                    (*jug)->setDesempate(c_desempate.getValue());
                des.append(*jug);
                qDebug()<<"Carta Desempate: "<<c_desempate.getValue();
            }
        }
         high=0;
         for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end(); jug++){
              if((*jug)->getDesempate() > high)
                  high=(*jug)->getDesempate();
         }
         qDebug()<<"Mayor desempate: "<<high;
         aux=0;
         for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end(); jug++)
             if((*jug)->getDesempate() == high){
                 aux++;
             }
         qDebug()<<"Aux desempate: "<<aux;
         if(aux>1){
             desempateFinal();
         }
         else
             return;
    }
}
void game::finishClientGame(int cards, int rounds, bool empate, QVector<int> *ids, QVector<int> *points){
    qDebug() << "Game over " << ids->count();
    panel_es->setData(cards, rounds, empate?"Si":"No");
    for(int var = 0; var < ids->count(); var++){
        bool out = false;
        //qDebug() <<
        for(QVector <nplayer*>::iterator jug = this->jugadores->begin(); jug != this->jugadores->end() && !out; jug++)
            if(ids->at(var) == (*jug)->getId()){
                panel_es->addItem((*jug)->getName(), points->at(var), 0);
                out = true;
            }
    }
    panel_es->setVisible(true);
    status = protocolo::fin;
}

void game::goInitInterface(){
    emit goInit();
}
