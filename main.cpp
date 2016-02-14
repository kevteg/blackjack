#include "blackmain.h"
#include <QApplication>

/**
 * @brief Clase inicial donde se declara el blackmain
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    blackmain w;
    w.show();

    return a.exec();
}
