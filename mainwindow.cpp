#include "mainwindow.h"

MainWindow::MainWindow() {
    renderer = new Renderer(RENDERER_WIDTH, RENDERER_HEIGHT);

    QWidget *mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(mainWidget);

    QWidget *objectParametersView = initializeObjectParameters();
    mainLayout->addWidget(objectParametersView);

    scrollArea = initializeWorkPane();
    mainLayout->addWidget(scrollArea);

    QWidget *parameters = initializeParametersView();
    mainLayout->addWidget(parameters);

    initializeMenu();
}

QWidget *MainWindow::initializeObjectParameters() {
    QWidget *objectParametersView = new QWidget(this);
    objectParametersView->setFixedWidth(150);

    QVBoxLayout *objectParametersLayout = new QVBoxLayout(objectParametersView);
    objectsList = new QListWidget(this);

    objectParametersLayout->addWidget(objectsList);

    objectParametersLayout->addWidget(sizeView = new VectorView(this, "Size"));
    objectParametersLayout->addWidget(positionView = new VectorView(this, "Position"));
    objectParametersLayout->addWidget(directionView = new VectorView(this, "Turn angle"));

    changeObjectParametersButton = new QPushButton("OK", this);

    connect(changeObjectParametersButton, SIGNAL(clicked()), this, SLOT(changeObjectParameters()));
    connect(objectsList, SIGNAL(currentRowChanged(int)), this, SLOT(changeCurrentObject()));
    connect(objectsList, SIGNAL(itemActivated(QListWidgetItem *)), this, SLOT(removeCurrentObject()));

    objectParametersLayout->addWidget(changeObjectParametersButton);
    changeCurrentObject();
    return objectParametersView;
}

QWidget *MainWindow::initializeParametersView() {
    QWidget *parameters = new QWidget(this);
    QVBoxLayout *parametersLayout = new QVBoxLayout(parameters);

    lightVectorView = new VectorView(parameters, "Light direction", -1.0, 1.0);
    lightVectorView->setValue(renderer->getLightVector());
    parametersLayout->addWidget(lightVectorView);

    centerVectorView = new VectorView(parameters, "Center position");
    centerVectorView->setValue(renderer->getCenter());
    parametersLayout->addWidget(centerVectorView);

    upVectorView = new VectorView(parameters, "Up direction", -1.0, 1.0);
    upVectorView->setValue(renderer->getUp());
    parametersLayout->addWidget(upVectorView);

    eyeVectorView = new VectorView(parameters, "Eye position");
    eyeVectorView->setValue(renderer->getEye());
    parametersLayout->addWidget(eyeVectorView);

    return parameters;
}

QScrollArea *MainWindow::initializeWorkPane() {
    QScrollArea *scrollArea = new QScrollArea(this);
    imageLabel = new QLabel(scrollArea);
    scrollArea->setWidget(imageLabel);
    scrollArea->setAlignment(Qt::AlignCenter);
    return scrollArea;
}

void MainWindow::initializeMenu() {
    refreshAction = new QAction(tr("&Refresh"), this);
    refreshAction->setShortcut(tr("Ctrl+R"));
    connect(refreshAction, SIGNAL(triggered()), this, SLOT(refresh()));

    QAction *addAction = new QAction("&Add", this);
    addAction->setShortcut(tr("Ctrl+A"));
    connect(addAction, SIGNAL(triggered()), this, SLOT(addNewObject()));

    actionMenu = new QMenu(tr("&Actions"), this);
    actionMenu->addAction(refreshAction);
    actionMenu->addAction(addAction);

    menuBar()->addMenu(actionMenu);
}

void MainWindow::refresh() {
    if (!renderer) return;
    QTime startTime = QTime::currentTime();
    renderer->setCenter(centerVectorView->getValue());
    renderer->setUp(upVectorView->getValue());
    renderer->setEye(eyeVectorView->getValue());
    renderer->setLightVector(lightVectorView->getValue());
    QPixmap renderedPixmap = renderer->render();
    imageLabel->setPixmap(renderedPixmap);
    imageLabel->resize(imageLabel->pixmap()->size());
    showTime(startTime.elapsed());
}

void MainWindow::showTime(int time) {
    QMessageBox messageBox(this);
    QString text;
    text.sprintf("%.2f sec", time / 1000.0);
    messageBox.setText(text);
    messageBox.exec();
}


void MainWindow::changeObjectParameters() {
    int index = objectsList->currentIndex().row();
    ObjectModel *object = renderer->getObject(index);
    object->setSize(sizeView->getValue());
    object->setPosition(positionView->getValue());
    object->setDirection(directionView->getValue());
}

void MainWindow::changeCurrentObject() {
    int index = objectsList->currentIndex().row();
    if (index == -1) {
        changeObjectParametersButton->setEnabled(false);
        return;
    } else {
        changeObjectParametersButton->setEnabled(true);
    }
    ObjectModel *object = renderer->getObject(index);
    sizeView->setValue(object->getSize());
    positionView->setValue(object->getPosition());
    directionView->setValue(object->getDirection());
}

void MainWindow::removeCurrentObject() {
    int index = objectsList->currentIndex().row();
    QListWidgetItem *item = objectsList->takeItem(index);
    delete item;
    renderer->removeObject(index);
}

void MainWindow::addObject(ObjectModel *object) {
    renderer->addObject(object);
    objectsList->addItem(object->getName());
}

void MainWindow::addNewObject() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open object", 0, tr("Warefront object files (*.obj)"));
    if (filePath != 0)
        addObject(ObjectModel::fromWareFrontObjectFile(filePath));
}
