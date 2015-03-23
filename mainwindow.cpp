#include "mainwindow.h"

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
    QTime startTime = QTime::currentTime();
    QPixmap renderedPixmap = renderer->render();
    showTime(startTime.elapsed());
    imageLabel->setPixmap(renderedPixmap);
    imageLabel->resize(imageLabel->pixmap()->size());
}

void MainWindow::showTime(int time) {
    QMessageBox messageBox(this);
    QString text;
    text.sprintf("%.2f sec", time / 1000.0);
    messageBox.setText(text);
    messageBox.exec();
}

