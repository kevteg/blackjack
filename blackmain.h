#ifndef BLACKMAIN_H
#define BLACKMAIN_H
/**
  Autores:
  Estefany Salas
  Kevin Hernández
  */
#include <QMainWindow>
#include <QToolButton>
#include <QTextEdit>
#include <QtCore>
#include <QMessageBox>
#include <QNetworkAddressEntry>
#include <QTimer>
#include "image.h"
#include "udpcomu.h"
#include "interfaz_inicio.h"
#include "interfaz_servidor.h"
#include "interfaz_cliente.h"
#include "server.h"
#include "client.h"
#include "panel_juego.h"
#include "protocolo.h"
#include "player.h"

/*Opción cliente, opción servidor*/
#define n_opc_ini 2
#define tcp_port 20060
#define _server 0
#define _client 1
namespace Ui {
    class blackmain;
    }
     /**
     * @brief La clase blackmain
     * Esta clase maneja y declara los objetos de comunicación, protócolo, gráficos y lógica del juego
     */
    class blackmain : public QMainWindow{
        Q_OBJECT

        public:
             /**
             * @brief constructor de blackmain
             * @param parent
             */
            explicit blackmain(QWidget *parent = 0);
             /**
             * @brief Crea las conexiones entre los botones y sus acciones
             */

            void setConnections();
            ~blackmain();
        protected:
            /**
             * @brief Carga la interfaz de usuario para el cliente
             */
            void loadClientInterface();
            /**
             * @brief Carga la interfaz de usuario para servidor
             */
            void loadServerInterface();
            void loadGameInterface();
        private slots:
            /**
             * @brief Evento de selección de cliente
             */
            void clientSelected();
            /**
             * @brief Evento de selección del usuario
             */
            void serverSelected();
            /**
             * @brief Se oprimio el about
             */
            void about();
            void goInitInterface();
            void processUdpData(QString sender_ip, QString data);
            void countServerTime();
            void noClients();
            void connectToTcpClient(QString dir_ip);
            void messagesFromCLient(int socket_des, QString data);
            void setSocketIdToClient(int);
        private:

            Ui::blackmain            *ui;
            /*Botones para escoger modalidades de juego*/
            Network::udpComu         *com_udp;
            interfaz_inicio          *inter_ini;
            interfaz_servidor        *inter_ser;
            interfaz_cliente         *inter_cli;
            Network::Client          *cliente;
            Network::server          *servidor;
            QString                   local_ip;
            panel_juego              *panel_principal;
            int                       conteo_server;
            int                       conteo_clientes;
            QTimer                   *timer;
            QVector<player>           jugadores;

    };

#endif // BLACKMAIN_H
