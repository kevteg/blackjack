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
    static const int cod_presentacion    = 1;
    static const int cod_solicitud       = 2;
    static const int cod_aceptacion      = 3;
    static const int cod_inicio          = 4;
    static const int cod_envio_carta     = 5;
    static const int cod_ofrecer_carta   = 6;
    static const int cod_respuesta_carta = 7;
    static const int cod_comienzo_ronda  = 8;
    static const int cod_final_juego     = 9;
    static const int max_time            = 120;
    static const int max_players         = 4;
    static QByteArray protocolJson(int codigo, QVector<QVariant> *info ){
           QJsonObject info_s;
           QJsonDocument trama;
            switch (codigo) {
            case cod_presentacion:
                info_s["codigo"]   = cod_presentacion;
                info_s["nombre"]   = info->at(0).toString();
                info_s["tiempo"]   = info->at(1).toInt();
                info_s["espacios"] = info->at(2).toInt();
                qDebug() << "entra";
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
            case cod_inicio:
                info_s["codigo"]      = cod_aceptacion;
                info_s["aceptado"]    = info->at(0).toBool();
                info_s["direccion"]   = info->at(1).toString();
                info_s["id"]          = info->at(2).toInt();
                break;
            case cod_envio_carta:

                break;
            case cod_ofrecer_carta:

                break;
            case cod_respuesta_carta:

                break;
            case cod_comienzo_ronda:

                break;
            case cod_final_juego:

                break;
            default:
                break;
            }
            trama = QJsonDocument(info_s);
            return trama.toJson();
        }
};

#endif // PROTOCOLO_H
