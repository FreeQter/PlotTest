#include "widget.h"
#include <QGraphicsScene>
#include <QLineSeries>
#include <QTime>
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>

static QTime timeCounter;

Widget::Widget(QWidget *parent)
    : QGraphicsView(new QGraphicsScene, parent),
      mChart(nullptr),
      mMousePressed(false)
{
    timeCounter.start();
    mChart = new QChart;
    mChart->setMinimumSize(640, 480);
    mChart->setTitle("chart test");
    mChart->legend()->hide();
    setRenderHints(QPainter::Antialiasing);

    QLineSeries *series = new QLineSeries;
    for (int i = 0; i < 8000; i++) {
        series->append(i + 3, 5);
    }
    mChart->addSeries(series);
    mChart->createDefaultAxes();
    scene()->addItem(mChart);

    qDebug() << "init 8000 points: " << timeCounter.elapsed() << "ms";
}

Widget::~Widget()
{

}

void Widget::resizeEvent(QResizeEvent *event)
{
    if (scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
         mChart->resize(event->size());
    }
    QGraphicsView::resizeEvent(event);
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (mMousePressed) {
        QPoint dPos = event->pos() - mLastPos;
        mLastPos = event->pos();
        mChart->scroll(-dPos.x(), dPos.y());
    }
    QGraphicsView::mouseMoveEvent(event);
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    mMousePressed = true;
    mLastPos = event->pos();
    QGraphicsView::mousePressEvent(event);
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    mMousePressed = false;
    QGraphicsView::mouseReleaseEvent(event);
}

void Widget::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() > 0) {
        mChart->zoomIn();
    } else{
        mChart->zoomOut();
    }
    QGraphicsView::wheelEvent(event);
}
