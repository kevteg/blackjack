#ifndef INTERFAZ_INICIO_H
#define INTERFAZ_INICIO_H

#include <QWidget>
#include "image.h"
#include <QToolButton>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#define n_opc_ini 2
#define SERVIDOR 0
#define CLIENTE 1
namespace Ui {
class interfaz_inicio;
}

class interfaz_inicio : public QWidget
{
    Q_OBJECT

public:
    explicit interfaz_inicio(QWidget *parent = 0);
    ~interfaz_inicio();

private:
    Ui::interfaz_inicio *ui;
    QToolButton              **opciones_iniciales;
    /*Barra para seleccionar el nombre de usuario*/
    QTextEdit                *barra_username;
    image                    *imagen_inicio;
    QVBoxLayout *layout_1;
    //En este layout van los botones iniciales de selección
    QHBoxLayout *layout_2;
public:
    QTextEdit* getBarra();
private slots:
    void emitServidor();
    void emitCliente();
signals:
    void serverSelected();
    void clientSelected();
};

#endif // INTERFAZ_INICIO_H
