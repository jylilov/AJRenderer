#pragma once

#include <QtWidgets>
#include "renderer.h"
#include "vectorview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    static const uint RENDERER_WIDTH = 600;
    static const uint RENDERER_HEIGHT = 600;
public:
    MainWindow();
private:
    QScrollArea *scrollArea;
    QLabel *imageLabel;

    QAction *refreshAction;

    QMenu *actionMenu;

    QListWidget *objectsList;

    VectorView *positionView;
    VectorView *sizeView;
    VectorView *directionView;
    QPushButton *changeObjectParametersButton;

    VectorView *lightVectorView;
    VectorView *upVectorView;
    VectorView *centerVectorView;
    VectorView *eyeVectorView;

    Renderer *renderer;

    void showTime(int time);
    void initializeMenu();
    QScrollArea *initializeWorkPane();
    QWidget *initializeParametersView();
    QWidget *initializeObjectParameters();
    void initializeRenderer();
private slots:
    void refresh();
    void changeObjectParameters();
    void removeCurrentObject();
    void changeCurrentObject();
    void addNewObject();
public:
    void addObject(ObjectModel *object);
};