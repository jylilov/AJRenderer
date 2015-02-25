#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow win;
    Renderer renderer(600, 600);
    WarefrontObject object("/home/jylilov/TEMP/african_head.obj");
    renderer.addObject(&object);

    win.setRenderer(&renderer);

    win.showMaximized();

    return app.exec();
}