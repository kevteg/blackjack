#ifndef PANEL_ESTADISTICAS_H
#define PANEL_ESTADISTICAS_H

#include <QWidget>
#include "protocolo.h"
#include "item_lista.h"
#include "nplayer.h"

namespace Ui {
class panel_estadisticas;
}

class panel_estadisticas : public QWidget
{
    Q_OBJECT

public:
    explicit panel_estadisticas(int cartas, int rounds, QWidget *parent = 0);
    void addItem(QString name, int points);
    ~panel_estadisticas();
public slots:
    void botonPressed();
signals:
    void goInit();
private:
    Ui::panel_estadisticas *ui;
    QVector<item_lista *> items;
    int color;
};

#endif // PANEL_ESTADISTICAS_H
