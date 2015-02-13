#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QLabel>
#include "renderer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    Renderer *getRenderer() const { return renderer; }
    void setRenderer(Renderer *renderer) { MainWindow::renderer = renderer; }

private:
    QScrollArea *scrollArea;
    QLabel *imageLabel;

    QAction *refreshAction;

    QMenu *actionMenu;

    Renderer *renderer;

private slots:
    void refresh();

};

#endif