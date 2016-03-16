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
    explicit panel_estadisticas(QWidget *parent = 0);
    void setItems(QVector<nplayer *> *jugadores);
    ~panel_estadisticas();

private:
    Ui::panel_estadisticas *ui;
    QVector<item_lista *> items;
};

#endif // PANEL_ESTADISTICAS_H
