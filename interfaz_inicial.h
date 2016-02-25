#ifndef INTERFAZ_INICIAL_H
#define INTERFAZ_INICIAL_H

#include <QWidget>

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
};

#endif // INTERFAZ_INICIAL_H
