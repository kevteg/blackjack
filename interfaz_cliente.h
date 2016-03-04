#ifndef INTERFAZ_CLIENTE_H
#define INTERFAZ_CLIENTE_H

#include <QWidget>
#include <QDebug>
#include <QListWidgetItem>
#include <QVector>
#include <QString>
#include "serveritem.h"
#define separador_string "||||"
namespace Ui {
class interfaz_cliente;
}

class interfaz_cliente : public QWidget
{
    Q_OBJECT

public:
    explicit interfaz_cliente(QWidget *parent = 0);
    ~interfaz_cliente();
    void addinListServer(QString server_name, QString server_ip, int time, int space);

private slots:
    void atras();
    void verifyConnectedServer(QString datos_servidor);
signals:
    void conexionTcpCliente(QString dir_ip);
    void goInitInterface();
private:
    Ui::interfaz_cliente *ui;
    int                   count_servers;
    QVector <serverItem*> servidores;
};

#endif // INTERFAZ_CLIENTE_H
