#ifndef INTERFAZ_INICIAL_H
#define INTERFAZ_INICIAL_H

#include <QWidget>
#include "image.h"
#include <QToolButton>
#include <QTextEdit>
#include <QHBoxLayout>

#define n_opc_ini 2

namespace Ui {
    class interfaz_inicial;
}

class interfaz_inicial : public QWidget
{
        Q_OBJECT

    public:
        explicit interfaz_inicial(QWidget *parent = 0);
        ~interfaz_inicial();

    private:
        Ui::interfaz_inicial *ui;
        image                *imagen_inicial;
        QToolButton          **opciones_iniciales;
        QTextEdit             *barra_username;
        QHBoxLayout           *layout_buttons;
};

#endif // INTERFAZ_INICIAL_H
