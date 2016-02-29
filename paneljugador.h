#ifndef PANELJUGADOR_H
#define PANELJUGADOR_H

#include <QWidget>

namespace Ui {
class panelJugador;
}

class panelJugador : public QWidget
{
    Q_OBJECT

public:
    explicit panelJugador(QWidget *parent = 0);
    ~panelJugador();

private:
    Ui::panelJugador *ui;
};

#endif // PANELJUGADOR_H
