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
#include <QVBoxLayout>
#include "interfaz_inicial.h"
#include "image.h"
#include "communication.h"
//#include "MultipurposeSocket_ReadyToUse/MultiPropuseSockets.h"
/*Opción cliente, opción servidor*/
#define n_opc_ini 2
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
        private:

            Ui::blackmain            *ui;
            /*Botones para escoger modalidades de juego*/
            QToolButton              **opciones_iniciales;
            /*Barra para seleccionar el nombre de usuario*/
            QTextEdit                *barra_username;
            image                    *imagen_inicio;
            communication             obj_com;
            /*Cosas gráficas*/
            QVBoxLayout *layout_1;
            QHBoxLayout *layout_2;
            interfaz_inicial *ini;
    };

#endif // BLACKMAIN_H
