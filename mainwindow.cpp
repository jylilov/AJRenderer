#include "mainwindow.h"

MainWindow::MainWindow() {
    renderer = new Renderer(RENDERER_WIDTH, RENDERER_HEIGHT);

    QWidget *mainWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(mainWidget);

    setCentralWidget(mainWidget);

    initializeObjectParametersView(mainWidget);
    initializeRendererView(mainWidget);
    initializeRendererParametersView(mainWidget);
    initializeMenu();
}

void MainWindow::initializeObjectParametersView(QWidget *parent) {
    QWidget *objectParametersView = new QWidget(parent);
    objectParametersView->setFixedWidth(200);

    QVBoxLayout *objectParametersLayout = new QVBoxLayout(objectParametersView);

    objectsListWidget = new QListWidget(objectParametersView);
    changeObjectParametersButton = new QPushButton("Change parameters", objectParametersView);

    objectParametersLayout->addWidget(objectsListWidget);
    objectParametersLayout->addWidget(objectSizeView = new VectorView(objectParametersView, "Size"));
    objectParametersLayout->addWidget(objectPositionView = new VectorView(objectParametersView, "Position"));
    objectParametersLayout->addWidget(objectDirectionView = new VectorView(objectParametersView, "Turn angle"));
    objectParametersLayout->addWidget(changeObjectParametersButton);

    connect(changeObjectParametersButton, SIGNAL(clicked()), this, SLOT(updateObjectParameters()));
    connect(objectsListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(updateCurrentObjectSelection()));
    connect(objectsListWidget, SIGNAL(itemActivated(QListWidgetItem *)), this, SLOT(removeCurrentObject()));

    updateCurrentObjectSelection();

    parent->layout()->addWidget(objectParametersView);
}

void MainWindow::initializeRendererParametersView(QWidget *parent) {
    QWidget *parametersView = new QWidget(parent);
    QVBoxLayout *parametersLayout = new QVBoxLayout(parametersView);

    rendererLightVectorView = new VectorView(parametersView, "Light direction", -1.0, 1.0);
    rendererLightVectorView->setValue(renderer->getLightVector());
    parametersLayout->addWidget(rendererLightVectorView);

    rendererCenterVectorView = new VectorView(parametersView, "Center position");
    rendererCenterVectorView->setValue(renderer->getCenterVector());
    parametersLayout->addWidget(rendererCenterVectorView);

    rendererUpVectorView = new VectorView(parametersView, "Up direction", -1.0, 1.0);
    rendererUpVectorView->setValue(renderer->getUpVector());
    parametersLayout->addWidget(rendererUpVectorView);

    rendererEyeVectorView = new VectorView(parametersView, "Eye position");
    rendererEyeVectorView->setValue(renderer->getEyeVector());
    parametersLayout->addWidget(rendererEyeVectorView);

    parent->layout()->addWidget(parametersView);
}

void MainWindow::initializeRendererView(QWidget *parent) {
    QScrollArea *rendererView = new QScrollArea(parent);
    rendererOutputLabel = new QLabel(rendererView);

    rendererView->setWidget(rendererOutputLabel);
    rendererView->setAlignment(Qt::AlignCenter);

    parent->layout()->addWidget(rendererView);
}

void MainWindow::initializeMenu() {
    refreshAction = new QAction(tr("&Refresh"), this);
    refreshAction->setShortcut(tr("Ctrl+R"));
    connect(refreshAction, SIGNAL(triggered()), this, SLOT(refresh()));

    addObjectAction = new QAction("&Add", this);
    addObjectAction->setShortcut(tr("Ctrl+A"));
    connect(addObjectAction, SIGNAL(triggered()), this, SLOT(addNewObject()));

    QMenu *actionMenu = new QMenu(tr("&Actions"), this);
    actionMenu->addAction(refreshAction);
    actionMenu->addAction(addObjectAction);

    menuBar()->addMenu(actionMenu);
}

void MainWindow::updateRendererParameters() {
    renderer->setCenterVector(rendererCenterVectorView->getValue());
    renderer->setUpVector(rendererUpVectorView->getValue());
    renderer->setEyeVector(rendererEyeVectorView->getValue());
    renderer->setLightVector(rendererLightVectorView->getValue());
}

void MainWindow::updateObjectParametersView(ObjectModel *object) {
    objectSizeView->setValue(object->getSize());
    objectPositionView->setValue(object->getPosition());
    objectDirectionView->setValue(object->getDirection());
}


void MainWindow::updateObjectParameters(ObjectModel *object) {
    object->setSize(objectSizeView->getValue());
    object->setPosition(objectPositionView->getValue());
    object->setDirection(objectDirectionView->getValue());
}

void MainWindow::showRenderTime(int time) {
    QMessageBox messageBox(this);
    QString text;
    text.sprintf("%.2f sec", time / 1000.0);
    messageBox.setText(text);
    messageBox.setWindowTitle("Render time");
    messageBox.exec();
}

void MainWindow::refresh() {
    updateRendererParameters();

    QTime startTime = QTime::currentTime();

    QPixmap renderedPixmap = renderer->render();
    rendererOutputLabel->setPixmap(renderedPixmap);
    rendererOutputLabel->resize(rendererOutputLabel->pixmap()->size());

    showRenderTime(startTime.elapsed());
}

void MainWindow::addObject(ObjectModel *object) {
    renderer->addObject(object);
    objectsListWidget->addItem(object->getName());
}

void MainWindow::updateObjectParameters() {
    int index = objectsListWidget->currentIndex().row();
    ObjectModel *object = renderer->getObject(index);
    updateObjectParameters(object);
}

void MainWindow::updateCurrentObjectSelection() {
    int index = objectsListWidget->currentIndex().row();
    if (index != -1) {
        changeObjectParametersButton->setEnabled(true);
        ObjectModel *object = renderer->getObject(index);
        updateObjectParametersView(object);
    } else {
        changeObjectParametersButton->setEnabled(false);
    }
}

void MainWindow::removeCurrentObject() {
    int index = objectsListWidget->currentIndex().row();
    QListWidgetItem *item = objectsListWidget->takeItem(index);
    delete item;
    renderer->removeObject(index);
}

void MainWindow::addNewObject() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open object", 0, tr("Warefront object files (*.obj)"));
    if (filePath != 0)
        addObject(ObjectModel::fromWareFrontObjectFile(filePath));
}
