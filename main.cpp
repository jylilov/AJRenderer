#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow win;
    Renderer renderer(600, 600);
    ObjectModel *object = ObjectModel::fromWareFrontObjectFile("/home/jylilov/TEMP/diablo3.obj");
    renderer.addObject(object);

    win.setRenderer(&renderer);

    win.showMaximized();

    int result = app.exec();

    delete object;

    return result;
}