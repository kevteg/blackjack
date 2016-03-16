#ifndef ITEM_LISTA_H
#define ITEM_LISTA_H

#include <QWidget>
#include "protocolo.h"


namespace Ui {
class item_lista;
}

class item_lista : public QWidget
{
    Q_OBJECT

public:
    explicit item_lista(QString name, int points, int desempate, int color, QWidget *parent = 0);
    void set(QString name, int points, int desempate, int color);
    ~item_lista();

private:
    Ui::item_lista *ui;
};

#endif // ITEM_LISTA_H
