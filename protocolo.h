#ifndef PROTOCOLO_H
#define PROTOCOLO_H
#include <QJsonObject>
#include <QJsonDocument>
#include <QVector>
#include <QVariant>
#include <QDebug>
#include <QJsonArray>

class protocolo : public QObject
{
    Q_OBJECT
public:
    static const int cod_saludo          = 1;
    static const int cod_solicitud       = 2;
    static const int cod_aceptacion      = 3;
    static const int cod_presentacion    = 4;
    static const int cod_comienzo_ronda  = 5;
    static const int cod_bono            = 6;
    static const int cod_ofrecer_carta   = 7;
    static const int cod_respuesta_carta = 8;
    static const int cod_envio_carta     = 9;
    static const int cod_final_juego     = 10;
    static const int cod_error           = 11;
    static const int max_time            = 120;
    static const int max_players         = 4;
    static QString dir_multicast;
    static const int servidor            = 0;
    static const int cliente             = 1;
    /*Estados de lo que está pasando: Esto es para que luego de que comience el juego no se pueda volver loco al cliente o al servidor con mensajes erróneos*/
    static const int nothing             = 0;
    static const int waiting_clients     = 1;
    static const int waiting_server_res  = 2;
    static const int playing             = 3;
    static const int waiting_game_to_start = 4;
    static const int tcp_port              = 20060;

    //Puertos
    static const int udp_port              = 20050;
    static const int multi_port            = 20070;
    static const int udpTtl                = 2;
    //Estados del juego como servidor
    static const int comienzo_ronda        = 0; //Para esperar los 30 segundos
    static const int ronda                 = 1;
    static const int fin                   = 2;
    //Tiempos del juego
    static const int tiempo_inicio_ronda   = 1*1000;
    static const int tiempo_espera_carta   = 1*1000;
    static const int gray                  = 0;
    static const int white                 = 1;


    static QByteArray generateJson(int codigo, QVector<QVariant> *info ){
           QJsonObject info_s;
           QJsonDocument trama;
            switch (codigo) {
            case cod_saludo:
                info_s["codigo"]   = cod_saludo;
                info_s["nombre"]   = info->at(0).toString();
                info_s["tiempo"]   = QString::number(info->at(1).toInt());
                info_s["espacios"] = QString::number(info->at(2).toInt());
                break;
            case cod_solicitud:
                info_s["codigo"] = cod_solicitud;
                info_s["nombre"] = info->at(0).toString();
                break;
            case cod_aceptacion:
                info_s["codigo"]      = cod_aceptacion;
                info_s["aceptado"]    = (info->at(0).toBool())?"true":"false";
                info_s["direccion"]   = info->at(1).toString();
                info_s["id"]          = QString::number(info->at(2).toInt());
                break;
            case cod_presentacion:{
                QJsonArray jugadores;
                int i = 0;
                info_s["codigo"]      = cod_presentacion;
                QJsonObject arreglo_interno;
                for(QVector <QVariant>::iterator variable = info->begin(); variable != info->end(); variable++){
                    for (int var = 0; var < arreglo_interno.count() && !i; ++var)
                        arreglo_interno.remove((!var)?"nombre":"id_asignado");
                    if(!i)
                        arreglo_interno["nombre"] = variable->toString();
                    else
                        arreglo_interno["id_asignado"] = QString::number(variable->toInt());
                    i++;
                    if(i == 2){
                        jugadores.append(arreglo_interno);
                        i = 0;
                    }
                   }
                info_s["jugadores"] = jugadores;
                break;
            }
            case cod_comienzo_ronda:{
                info_s["codigo"]      = cod_comienzo_ronda;
                QJsonObject arreglo_interno;
                QJsonArray puntaje;
                int i = 0;
                for(QVector <QVariant>::iterator variable = info->begin(); variable != info->end(); variable++){
                    for (int var = 0; var < arreglo_interno.count() && !i; ++var)
                        arreglo_interno.remove((!var)?"id":"puntaje");
                    if(!i)
                        arreglo_interno["id"]  = variable->toString();
                    else
                        arreglo_interno["puntaje"] = variable->toInt();
                    i++;
                    if(i == 2){
                        puntaje.append(arreglo_interno);
                        i = 0;
                    }
                   }
                info_s["puntaje"] = puntaje;
                break;
            }
            case cod_bono:
                info_s["codigo"]      = cod_bono;
                info_s["id"]          = QString::number(info->at(0).toInt());
                info_s["bono"]        = info->at(1).toBool()?"true":"false";
                break;
            case cod_ofrecer_carta:
                info_s["codigo"]      = cod_ofrecer_carta;
                info_s["id"]          = QString::number(info->at(0).toInt());
                break;
            case cod_respuesta_carta:
                info_s["codigo"]      = cod_respuesta_carta;
                info_s["jugar"]       = info->at(0).toBool()?"true":"false";
                break;
            case cod_envio_carta:
                info_s["codigo"]      = cod_envio_carta;
                info_s["id"]          = QString::number(info->at(0).toInt());
                info_s["carta"]       = info->at(1).toString();
                break;

            case cod_final_juego:{
                info_s["codigo"]            = cod_final_juego;
                info_s["rondas"]            = QString::number(info->at(0).toInt());
                QJsonObject arreglo_interno;
                QJsonArray puntaje;
                int i = 0;
                for(int _var = 2; _var < info->count(); _var++) {
                    for (int var = 0; var < arreglo_interno.count() && !i; ++var)
                        arreglo_interno.remove((!var)?"id":"puntaje");
                    if(!i)
                        arreglo_interno["id"]  = info->at(_var).toString();
                    else
                        arreglo_interno["puntaje"] = QString::number(info->at(_var).toInt());
                    i++;
                    if(i == 2){
                        puntaje.append(arreglo_interno);
                        i = 0;
                    }
                }
                /*for(QVector <QVariant>::iterator variable = info->begin(); variable != info->end(); variable++){

                   }*/
                info_s["puntaje"]           = puntaje;
                info_s["cartas_jugadas"]    = QString::number(info->at(1).toInt());
                info_s["desempate"]         = info->at(3).toBool()?"true":"false";
                break;
            }
            case cod_error:
                info_s["codigo"]      = cod_error;
                info_s["id"]          = QString::number(info->at(0).toInt());
                break;
            default:
                break;
            }
            trama = QJsonDocument(info_s);
            //return trama.toJson().simplified();
            return trama.toJson();
        }
    static QVector<QVariant>* JsonToVector(QByteArray datos){
        QJsonDocument trama = QJsonDocument::fromJson(datos);
         QVector<QVariant> *vector_datos = NULL;
        if(trama.isObject()){
            vector_datos = new QVector<QVariant>;
             switch (trama.object()["codigo"].toInt()) {
             case cod_saludo:
                 vector_datos->append(cod_saludo);
                 vector_datos->append(trama.object()["nombre"].toString());
                 vector_datos->append(trama.object()["tiempo"].toString().toInt());
                 vector_datos->append(trama.object()["espacios"].toString().toInt());
                 break;
             case cod_solicitud:
                 vector_datos->append(cod_solicitud);
                 vector_datos->append(trama.object()["nombre"].toString());
                 break;
             case cod_aceptacion:
                 vector_datos->append(cod_aceptacion);
                 vector_datos->append((trama.object()["aceptado"].toString() == "true")?true:false);
                 vector_datos->append(trama.object()["direccion"].toString());
                 vector_datos->append(trama.object()["id"].toString().toInt());
                 break;
             case cod_presentacion:{
                 vector_datos->append(cod_presentacion);
                 QJsonArray arreglo_interno;
                 arreglo_interno = trama.object()["jugadores"].toArray();
                 for(int var = 0; var < arreglo_interno.size(); var++){
                     QJsonObject obj = arreglo_interno[var].toObject();
                     for(int i = 0; i < obj.size(); i++){
                         if(!i)
                           vector_datos->append(obj["nombre"].toString());
                         else
                           vector_datos->append(obj["id_asignado"].toString().toInt());
                     }
                 }
                 break;
                }
             case cod_comienzo_ronda:{
                 vector_datos->append(cod_comienzo_ronda);
                 QJsonArray arreglo_interno;
                 arreglo_interno = trama.object()["puntaje"].toArray();
                 for(int var = 0; var < arreglo_interno.size(); var++){
                     QJsonObject obj = arreglo_interno[var].toObject();
                     for(int i = 0; i < obj.size(); i++){
                         if(!i)
                           vector_datos->append(obj["id"].toString());
                         else
                           vector_datos->append(obj["puntaje"].toInt());
                     }
                 }
                 break;
                }
             case cod_bono:
                 vector_datos->append(cod_bono);
                 vector_datos->append(trama.object()["id"].toString().toInt());
                 vector_datos->append((trama.object()["bono"].toString() == "true")?true:false);
                 break;
             case cod_ofrecer_carta:
                 vector_datos->append(cod_ofrecer_carta);
                 vector_datos->append(trama.object()["id"].toString().toInt());
                 break;
             case cod_respuesta_carta:
                 vector_datos->append(cod_respuesta_carta);
                 vector_datos->append((trama.object()["jugar"].toString() == "true")?true:false);
                 break;

             case cod_envio_carta:
                 vector_datos->append(cod_envio_carta);
                 vector_datos->append(trama.object()["id"].toString().toInt());
                 vector_datos->append(trama.object()["carta"].toString());
                 break;

             case cod_final_juego:{
                 vector_datos->append(cod_final_juego);
                 vector_datos->append(trama.object()["rondas"].toString().toInt());
                 vector_datos->append(trama.object()["cartas_jugadas"].toString().toInt());
                 vector_datos->append(QString::number((trama.object()["desempate"].toString() == "true")?true:false));
                 QJsonArray arreglo_interno;
                 arreglo_interno = trama.object()["puntaje"].toArray();
                 for(int var = 0; var < arreglo_interno.size(); var++){
                     QJsonObject obj = arreglo_interno[var].toObject();
                     for(int i = 0; i < obj.size(); i++){
                         if(!i)
                           vector_datos->append(obj["id"].toString().toInt());
                         else
                           vector_datos->append(obj["puntaje"].toString().toInt());
                     }
                 }
                 break;
             }
             case cod_error:
                 vector_datos->append(cod_error);
                 vector_datos->append(trama.object()["id"].toString().toInt());
                 break;
             default:
                 break;
             }
        }
         return vector_datos;
    }
};

#endif // PROTOCOLO_H
