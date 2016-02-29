#ifndef INTERFAZ_CLIENTE_H
#define INTERFAZ_CLIENTE_H

#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <QListWidgetItem>
#include <QVector>
#include <QString>
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
    void addinListServer(QString server_name, QString server_ip, int server_port);

private slots:
    void atras();
    void limpiarListaServidores();
    void itemPresionado();
    void verifyConnectedServer();
signals:
    void conexionTcpCliente(QString dir_ip);
    void goInitInterface();
private:
    Ui::interfaz_cliente *ui;
    QTimer               *timer;
    QVector<QString>      servidores;
};

#endif // INTERFAZ_CLIENTE_H
