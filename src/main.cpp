#include "mainwindow.h"

#include <QApplication>

//Don't touch this unless you know what you are doing
int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Chips and Cheese Benchmarks");
    w.show();
    return QApplication::exec();
}
