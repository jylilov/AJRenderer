#include <QApplication>
#include "mainwindow.h"
#include "matrix.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow win;
    Renderer renderer(600, 600);
    WarefrontObject object("/home/jylilov/TEMP/african_head.obj");
    renderer.addObject(&object);
    double array[] = {
            0, 0, 0, 1,
            0, 0, 0, 1,
            1, 0, 0, 0,
            1, 0, 0, 0
    };

    Mat4d matrix(array);
    Matrix<3, 3, double> m3 = matrix.getMinor(2, 2);

    qDebug() << matrix.getDeterminant();


    win.setRenderer(&renderer);

    win.showMaximized();

    return app.exec();
}