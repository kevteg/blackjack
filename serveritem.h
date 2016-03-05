#ifndef SERVERITEM_H
#define SERVERITEM_H

#include <QWidget>
#include "protocolo.h"
namespace Ui {
class serverItem;
}

class serverItem : public QWidget
{
    Q_OBJECT

public:
    explicit serverItem(QString server_name, QString ip, int time, int space, QWidget *parent = 0);
    ~serverItem();
    void updateTime(int time);
    void updateSpace(int space);
    int getTime();
    QString getIp();
    QString getData();
signals:
    void seleccionServidor(QString);

public slots:
    void sendServerData();

private:
    QString ip;
    QString name;
    int time;
    int space;
    Ui::serverItem *ui;

};

#endif // SERVERITEM_H
