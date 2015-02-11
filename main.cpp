#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow win;
    Renderer renderer(600, 600);

    win.setRenderer(&renderer);

    win.showMaximized();

    return app.exec();
}