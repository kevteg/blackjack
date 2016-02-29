#ifndef INTERFAZ_CLIENTE_H
#define INTERFAZ_CLIENTE_H

#include <QWidget>

namespace Ui {
class interfaz_cliente;
}

class interfaz_cliente : public QWidget
{
    Q_OBJECT

public:
    explicit interfaz_cliente(QWidget *parent = 0);
    ~interfaz_cliente();
    void addinListServer(QString new_server);
private slots:
    void atras();
signals:
    void goInitInterface();
private:
    Ui::interfaz_cliente *ui;
};

#endif // INTERFAZ_CLIENTE_H
