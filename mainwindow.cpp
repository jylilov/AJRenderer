#include "mainwindow.h"

#include <QtWidgets>

MainWindow::MainWindow() {
    imageLabel = new QLabel();

    scrollArea = new QScrollArea();
    scrollArea->setWidget(imageLabel);
    scrollArea->setAlignment(Qt::AlignCenter);

    setCentralWidget(scrollArea);

    refreshAction = new QAction(tr("&Refresh"), this);
    refreshAction->setShortcut(tr("Ctrl+R"));
    connect(refreshAction, SIGNAL(triggered()), this, SLOT(refresh()));

    actionMenu = new QMenu(tr("&Actions"), this);
    actionMenu->addAction(refreshAction);

    menuBar()->addMenu(actionMenu);
}

void MainWindow::refresh() {
    if (!renderer) return;

    imageLabel->setPixmap(renderer->render());
    imageLabel->resize(imageLabel->pixmap()->size());
}

