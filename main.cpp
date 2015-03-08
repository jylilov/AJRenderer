#include <QApplication>
#include "mainwindow.h"

//static const QString OBJECT = "/home/jylilov/TEMP/beetle.obj";

//static const QString OBJECT = "/home/jylilov/TEMP/diablo3.obj";
//static const QString DIFFUSE_TEXTURE = "/home/jylilov/TEMP/diablo3_diffuse.png";
//static const QString NM_TEXTURE = "/home/jylilov/TEMP/diabl/o3_nm.png";
//static const QString SPEC_TEXTURE = "/home/jylilov/TEMP/diablo3_spec.png";

static const QString OBJECT = "/home/jylilov/TEMP/african_head.obj";
static const QString DIFFUSE_TEXTURE = "/home/jylilov/TEMP/african_head_diffuse.png";
static const QString NM_TEXTURE = "/home/jylilov/TEMP/african_head_nm.png";
static const QString SPEC_TEXTURE = "/home/jylilov/TEMP/african_head_spec.png";

static const uint WIDTH = 600;
static const uint HEIGHT = 600;

static const double MODEL_SIZE = 0.85;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow win;
    Renderer renderer(WIDTH, HEIGHT);

    ObjectModel *object = ObjectModel::fromWareFrontObjectFile(OBJECT);
    Texture *diffuseTexture = Texture::fromFile(DIFFUSE_TEXTURE);
    Texture *nmTexture = Texture::fromFile(NM_TEXTURE);
    Texture *specTexture = Texture::fromFile(SPEC_TEXTURE);

    object->setSize(MODEL_SIZE);
    object->setDiffuseTexture(diffuseTexture);
    object->setNmTexture(nmTexture);
    object->setSpecTexture(specTexture);

    renderer.addObject(object);

    win.setRenderer(&renderer);

    win.showMaximized();

    int result = app.exec();

    delete object;
    delete diffuseTexture;
    delete nmTexture;
    delete specTexture;

    return result;
}