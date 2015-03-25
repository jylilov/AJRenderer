#include <QApplication>
#include "mainwindow.h"

//static const QString OBJECT = "/home/jylilov/TEMP/beetle.obj";

//static const QString OBJECT = "/home/jylilov/TEMP/diablo3.obj";
//static const QString DIFFUSE_TEXTURE = "/home/jylilov/TEMP/diablo3_diffuse.png";
//static const QString NM_TEXTURE = "/home/jylilov/TEMP/diabl/o3_nm.png";
//static const QString SPEC_TEXTURE = "/home/jylilov/TEMP/diablo3_spec.png";

static const int OBJECTS_COUNT = 2;
static const QString OBJECTS[OBJECTS_COUNT][4] = {
        {"/home/jylilov/TEMP/african_head.obj",
        "/home/jylilov/TEMP/african_head_diffuse.png",
        "/home/jylilov/TEMP/african_head_nm.png",
        "/home/jylilov/TEMP/african_head_spec.png"},

        {"/home/jylilov/TEMP/african_head_eye_inner.obj",
        "/home/jylilov/TEMP/african_head_eye_inner_diffuse.png",
        "/home/jylilov/TEMP/african_head_eye_inner_nm.png",
        "/home/jylilov/TEMP/african_head_eye_inner_spec.png"}
//
//        ,{"/home/jylilov/TEMP/african_head_eye_outer.obj",
//        "/home/jylilov/TEMP/african_head_eye_outer_diffuse.png",
//        "/home/jylilov/TEMP/african_head_eye_outer_nm.png",
//        "/home/jylilov/TEMP/african_head_eye_outer_spec.png"}
};

static const uint WIDTH = 600;
static const uint HEIGHT = 600;

static const double MODEL_SIZE = 1;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow win;
    Renderer renderer(WIDTH, HEIGHT);

    for (int i = 0; i < OBJECTS_COUNT; ++i) {
        ObjectModel *object = ObjectModel::fromWareFrontObjectFile(OBJECTS[i][0]);
        Texture *diffuseTexture = Texture::fromFile(OBJECTS[i][1]);
        Texture *nmTexture = Texture::fromFile(OBJECTS[i][2]);
        Texture *specTexture = Texture::fromFile(OBJECTS[i][3]);

        object->setSize(MODEL_SIZE);
        object->setDiffuseTexture(diffuseTexture);
        object->setNmTexture(nmTexture);
        object->setSpecTexture(specTexture);

        renderer.addObject(object);
    }

    win.setRenderer(&renderer);

    win.showMaximized();

    int result = app.exec();

    return result;
}