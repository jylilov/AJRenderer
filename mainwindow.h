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

    QLabel *rendererOutputLabel;
    QAction *refreshAction;

    QAction *addObjectAction;
    QListWidget *objectsListWidget;

    VectorView *objectPositionView;

    VectorView *objectSizeView;
    VectorView *objectDirectionView;
    QPushButton *changeObjectParametersButton;
    VectorView *rendererLightVectorView;

    VectorView *rendererUpVectorView;
    VectorView *rendererCenterVectorView;
    VectorView *rendererEyeVectorView;
    Renderer *renderer;
public:
    MainWindow();
    ~MainWindow() { delete renderer; }
private:
    void initializeMenu();
    void initializeRendererView(QWidget *parent);
    void initializeRendererParametersView(QWidget *parent);
    void initializeObjectParametersView(QWidget *parent);

    void updateRendererParameters();
    void updateObjectParameters(ObjectModel *object);
    void updateObjectParametersView(ObjectModel *object);

    void showRenderTime(int time);
    void addObject(ObjectModel *object);
private slots:
    void refresh();
    void updateObjectParameters();
    void removeCurrentObject();
    void updateCurrentObjectSelection();

    void addNewObject();
};