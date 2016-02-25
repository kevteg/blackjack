#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <QWidget>

namespace Ui {
class servidor;
}

class servidor : public QWidget
{
    Q_OBJECT

public:
    explicit servidor(QWidget *parent = 0);
    ~servidor();

private:
    Ui::servidor *ui;
};

#endif // SERVIDOR_H
