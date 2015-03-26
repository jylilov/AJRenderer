#include <QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow win;
    win.showMaximized();

    int result = app.exec();

    return result;
}