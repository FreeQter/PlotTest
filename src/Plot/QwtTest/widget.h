#ifndef WIDGET_H
#define WIDGET_H

#include "qwt_plot.h"

class Widget : public QwtPlot
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
};

#endif // WIDGET_H
