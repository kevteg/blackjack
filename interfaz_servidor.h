#ifndef INTERFAZ_SERVIDOR_H
#define INTERFAZ_SERVIDOR_H

#include <QWidget>
#include <QString>
#include "protocolo.h"
#include "player.h"
namespace Ui {
class interfaz_servidor;
}

class interfaz_servidor : public QWidget
{
    Q_OBJECT

public:
    explicit interfaz_servidor(QWidget *parent = 0);
    ~interfaz_servidor();
    void updateTime(int time);
    int getTime();
private slots:
    void atras();
    void addClientToList(player new_gamer);
signals:
    void goInitInterface();
private:
    Ui::interfaz_servidor *ui;
    int time;
};

#endif // INTERFAZ_SERVIDOR_H
