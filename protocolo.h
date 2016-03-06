#ifndef PROTOCOLO_H
#define PROTOCOLO_H
#include <QJsonObject>
#include <QJsonDocument>
#include <QVector>
#include <QVariant>
#include <QDebug>
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
    /*Estados de lo que está pasando: Esto es para que luego de que comience el juego no se pueda volver loco al cliente o al servidor con mensajes erróneos*/
    static const int nothing             = 0;
    static const int waiting_clients     = 1;
    static const int waiting_server_res  = 2;
    static const int playing             = 3;
    static const int waiting_game_to_start = 4;

    static QByteArray generateJson(int codigo, QVector<QVariant> *info ){
           QJsonObject info_s;
           QJsonDocument trama;
            switch (codigo) {
            case cod_saludo:
                info_s["codigo"]   = cod_saludo;
                info_s["nombre"]   = info->at(0).toString();
                info_s["tiempo"]   = info->at(1).toInt();
                info_s["espacios"] = info->at(2).toInt();
                break;
            case cod_solicitud:
                info_s["codigo"] = cod_solicitud;
                info_s["nombre"] = info->at(0).toString();
                break;
            case cod_aceptacion:
                info_s["codigo"]      = cod_aceptacion;
                info_s["aceptado"]    = info->at(0).toBool();
                info_s["direccion"]   = info->at(1).toString();
                info_s["id"]          = info->at(2).toInt();
                break;
            case cod_presentacion:
                info_s["codigo"]      = cod_presentacion;
                //info_s[] array jugadores
                break;
            case cod_envio_carta:
                info_s["codigo"]      = cod_envio_carta;
                info_s["id"]          = info->at(0).toInt();
                info_s["carta"]       = info->at(1).toString();
                break;
            case cod_bono:
                info_s["codigo"]      = cod_bono;
                info_s["bono"]        = info->at(0).toBool();
                break;
            case cod_ofrecer_carta:
                info_s["codigo"]      = cod_ofrecer_carta;
                info_s["id"]          = info->at(0).toInt();
                break;
            case cod_respuesta_carta:
                info_s["codigo"]      = cod_respuesta_carta;
                info_s["jugar"]       = info->at(0).toBool();
                break;
            case cod_comienzo_ronda:
                info_s["codigo"]      = cod_comienzo_ronda;
                //info_s[] ARRAY
                break;
            case cod_final_juego:
                info_s["codigo"]            = cod_final_juego;
                info_s["rondas"]            = info->at(0).toInt();
                info_s["cartas_jugadas"]    = info->at(1).toInt();
                //info_s[""] Array puntajes
                info_s["desempate"]         = info->at(3).toBool();
                break;
            case cod_error:
                info_s["codigo"]      = cod_error;
                info_s["id"]          =info->at(0).toInt();
                break;
            default:
                break;
            }
            trama = QJsonDocument(info_s);
            return trama.toJson();
        }
    static QVector<QVariant>* JsonToVector(QByteArray datos){
        QJsonDocument trama = QJsonDocument::fromJson(datos);
        QVector<QVariant> *vector_datos = new QVector<QVariant>;
         switch (trama.object()["codigo"].toInt()) {
         case cod_saludo:
             vector_datos->append(cod_saludo);
             vector_datos->append(trama.object()["nombre"].toString());
             vector_datos->append(trama.object()["tiempo"].toInt());
             vector_datos->append(trama.object()["espacios"].toInt());
             break;
         case cod_solicitud:
             vector_datos->append(cod_solicitud);
             vector_datos->append(trama.object()["nombre"].toString());
             break;
         case cod_aceptacion:
             vector_datos->append(cod_aceptacion);
             vector_datos->append(trama.object()["aceptado"].toBool());
             vector_datos->append(trama.object()["direccion"].toString());
             vector_datos->append(trama.object()["id"].toInt());
             break;
         case cod_presentacion:

             break;
         case cod_envio_carta:

             break;
         case cod_bono:

             break;
         case cod_ofrecer_carta:

             break;
         case cod_respuesta_carta:

             break;
         case cod_comienzo_ronda:

             break;
         case cod_final_juego:

             break;
         case cod_error:

             break;
         default:
             break;
         }
         return vector_datos;
    }
};

#endif // PROTOCOLO_H
