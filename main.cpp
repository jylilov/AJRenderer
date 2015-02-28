#include <QApplication>
#include "mainwindow.h"

//#define OBJECT "/home/jylilov/TEMP/diablo3.obj"
//#define TEXTURE "/home/jylilov/TEMP/diablo3_diffuse.png"
//
#define OBJECT "/home/jylilov/TEMP/african_head.obj"
#define TEXTURE "/home/jylilov/TEMP/african_head_diffuse.png"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow win;
    Renderer renderer(600, 600);

    ObjectModel *object = ObjectModel::fromWareFrontObjectFile(OBJECT);
    object->setSize(500);
    Texture *texture = new Texture(TEXTURE);
    object->setTexture(texture);

    renderer.addObject(object);

    win.setRenderer(&renderer);

    win.showMaximized();

    int result = app.exec();

    delete object;
    delete texture;

    return result;
}