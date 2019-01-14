#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "qcustomplot/qcustomplot.h"

class Widget : public QCustomPlot
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
};

#endif // WIDGET_H
