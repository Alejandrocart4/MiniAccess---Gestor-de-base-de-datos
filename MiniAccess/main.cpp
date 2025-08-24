#include <QApplication>
#include "vista_prueba.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    VistaPrueba w;
    w.setWindowTitle("MiniAccess - Vista tipo Access");
    w.resize(1360, 760);
    w.show();
    return app.exec();
}
