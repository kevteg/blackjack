#ifndef PANEL_JUEGO_H
#define PANEL_JUEGO_H

#include <QWidget>

namespace Ui {
class panel_juego;
}

class panel_juego : public QWidget
{
    Q_OBJECT

public:
    explicit panel_juego(QWidget *parent = 0);
    ~panel_juego();

private:
    Ui::panel_juego *ui;
};

#endif // PANEL_JUEGO_H
