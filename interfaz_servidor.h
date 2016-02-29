#ifndef INTERFAZ_SERVIDOR_H
#define INTERFAZ_SERVIDOR_H

#include <QWidget>
#include <QString>
namespace Ui {
class interfaz_servidor;
}

class interfaz_servidor : public QWidget
{
    Q_OBJECT

public:
    explicit interfaz_servidor(QWidget *parent = 0);
    ~interfaz_servidor();

private slots:
    void atras();
signals:
    void goInitInterface();
private:
    Ui::interfaz_servidor *ui;
};

#endif // INTERFAZ_SERVIDOR_H
