#include <QApplication>
#include "vista_prueba.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    VistaPrueba w;
    w.show();
    return a.exec();
}
