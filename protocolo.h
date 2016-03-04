#ifndef PROTOCOLO_H
#define PROTOCOLO_H


class protocolo
{
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
};

#endif // PROTOCOLO_H
