#include "vectorview.h"

VectorView::VectorView(QWidget *parent, QString name, double minValue, double maxValue) : QWidget(parent) {
    initializeLayout();
    initializeLabelView(name);
    xSpinBox = initializeVectorComponentView("x", minValue, maxValue);
    ySpinBox = initializeVectorComponentView("y", minValue, maxValue);
    zSpinBox = initializeVectorComponentView("z", minValue, maxValue);
}

void VectorView::initializeLayout() {
    formLayout = new QFormLayout(this);
}

void VectorView::initializeLabelView(QString &name) {
    label = new QLabel(name, this);
    formLayout->addRow(label);
}

Vec3d VectorView::getValue() {
    return Vec3d(
      xSpinBox->value(),
      ySpinBox->value(),
      zSpinBox->value()
    );
}

QDoubleSpinBox *VectorView::initializeVectorComponentView(QString name, double min, double max) {
    QDoubleSpinBox *spinBox = new QDoubleSpinBox(this);
    spinBox->setMinimum(min);
    spinBox->setMaximum(max);
    spinBox->setSingleStep(0.1);
    formLayout->addRow(name, spinBox);
    return spinBox;
}

void VectorView::setValue(Vec3d value) {
    xSpinBox->setValue(value[0]);
    ySpinBox->setValue(value[1]);
    zSpinBox->setValue(value[2]);
}
