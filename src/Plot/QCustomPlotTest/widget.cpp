#include "widget.h"
#include <QWheelEvent>
#include <QTime>
#include <QDebug>

static QTime timeCounter;
Widget::Widget(QWidget *parent)
    : QCustomPlot(parent)
{
    timeCounter.start();
    QVector<double> x(8001), y(8001);
    for (int i=0; i<8001; ++i)
    {
      x[i] = i;
      y[i] = 5;
    }
    this->addGraph();
    this->graph(0)->setData(x, y);

    this->xAxis->setLabel("x");
    this->yAxis->setLabel("y");

    this->xAxis->setRange(0, 8000);
    this->yAxis->setRange(0, 20);
    setMinimumSize(640, 480);

    connect(this, &Widget::mouseWheel, [=](QWheelEvent* e) {
        qreal scale = e->angleDelta().y() > 0? 0.9 : 1.1;
        this->xAxis->setRange(this->xAxis->range() * scale);
        this->yAxis->setRange(this->yAxis->range() * scale);
        this->replot();
        this->axisRect();
    });

    qDebug() << "init 8000 points: " << timeCounter.elapsed() << "ms";
}

Widget::~Widget()
{

}
