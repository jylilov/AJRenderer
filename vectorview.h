#pragma once

#include <QtWidgets>
#include "vector.h"

class VectorView : public QWidget {
private:
    static const int DEFAULT_MIN_VALUE = -1000;
    static const int DEFAULT_MAX_VALUE = 1000;
private:
    QLabel *label;

    QDoubleSpinBox *xSpinBox;
    QDoubleSpinBox *ySpinBox;
    QDoubleSpinBox *zSpinBox;

    QFormLayout *formLayout;
public:
    VectorView(QWidget *parent = 0, QString name = "", double minValue = DEFAULT_MIN_VALUE, double maxValue = DEFAULT_MAX_VALUE);
    Vec3d getValue();
    void setValue(Vec3d value);
private:
    QDoubleSpinBox *initializeVectorComponentView(QString name, double min, double max);
    void initializeLabelView(QString &name);
    void initializeLayout();
};
