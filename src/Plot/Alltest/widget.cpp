#include "widget.h"
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QChart>
#include <QIntValidator>
#include <QLineSeries>
#include <QtCore>
#include "../qcustomplot/qcustomplot.h"

#include "qwt_plot.h"
#include "qwt_legend.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_magnifier.h"
#include "qwt_plot_curve.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent),
      mLineCountLabel(nullptr),
      mPointCountLabel(nullptr),
      mLineCountEdit(nullptr),
      mPointCountEdit(nullptr),
      mPlotW(nullptr),
      mDrawBnt(nullptr),
      mPlotsGBnt(nullptr),
      mCurrentPlot(nullptr),
      mTimeLabel(nullptr)
{
    initUI();
    initConnections();
}

void Widget::initUI()
{
    // plot checkboxes
    QVBoxLayout* leftPanel = new QVBoxLayout;
    mPlotsGBnt = new QButtonGroup(this);
    QStringList plotNames;
    plotNames << "QtChart" << "QcustomPlot" << "QWT";
    leftPanel->addStretch(3);
    int idx = 0;
    foreach(const QString& plotName, plotNames) {
        QPushButton* bnt = new QPushButton(plotName, this);
        bnt->setCheckable(true);
        mPlotsGBnt->addButton(bnt);
        mPlotsGBnt->setId(bnt, idx);
        leftPanel->addWidget(bnt);
        leftPanel->addStretch(1);
        idx++;
    }
    leftPanel->addStretch(3);
    mPlotsGBnt->buttons().first()->setChecked(true);

    // opts
    QHBoxLayout* optLayout = new QHBoxLayout;
    mLineCountLabel = new QLabel("Line counts", this);
    mPointCountLabel = new QLabel("Point counts", this);
    mLineCountEdit = new QLineEdit(this);
    mLineCountEdit->setText("1");
    mLineCountEdit->setValidator(new QIntValidator);
    mPointCountEdit = new QLineEdit(this);
    mPointCountEdit->setText("100");
    mPointCountEdit->setValidator(new QIntValidator);
    mDrawBnt = new QPushButton("Start draw" ,this);
    optLayout->addStretch(3);
    optLayout->addWidget(mLineCountLabel);
    optLayout->addWidget(mLineCountEdit);
    optLayout->addStretch(1);
    optLayout->addWidget(mPointCountLabel);
    optLayout->addWidget(mPointCountEdit);
    optLayout->addStretch(3);
    optLayout->addWidget(mDrawBnt);
    optLayout->addStretch(1);

    // plot widgets
    QVBoxLayout* contentLayout = new QVBoxLayout;
    mPlotW = new QStackedWidget(this);
    contentLayout->addLayout(optLayout);
    contentLayout->addWidget(mPlotW);
    // setup plots
    APlot* plot = new QtChartPlot(this);
    mPlotW->addWidget(plot);
    mCurrentPlot = plot;
    mPlotW->addWidget(new CustumPlot(this));
    mPlotW->addWidget(new AQWtPlot(this));
    const int& drawTime = plot->startDraw(mLineCountEdit->text().toInt(), mPointCountEdit->text().toInt());

    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->addLayout(leftPanel);
    topLayout->addLayout(contentLayout);

    mTimeLabel = new QLabel("Draw time: " + QString::number(drawTime) + "ms", this);
    QHBoxLayout* timeLayout = new QHBoxLayout;
    timeLayout->addWidget(mTimeLabel, 0, Qt::AlignHCenter);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(timeLayout);

    setLayout(mainLayout);

    setMinimumSize(720, 640);
}

void Widget::initConnections()
{
    connect(mDrawBnt, &QPushButton::clicked, this, &Widget::onStartDraw);
    connect(mPlotsGBnt, SIGNAL(buttonClicked(int)), this, SLOT(onPlotSelected(int)));
}

Widget::~Widget()
{

}

void Widget::onStartDraw()
{
    if (mCurrentPlot) {
        const int& drawTime = mCurrentPlot->startDraw(mLineCountEdit->text().toInt(), mPointCountEdit->text().toInt());
        mTimeLabel->setText("Draw time: " + QString::number(drawTime) + "ms");
    }
}

void Widget::onPlotSelected(const int &id)
{
    mPlotW->setCurrentIndex(id);
    mCurrentPlot = qobject_cast<APlot*>(mPlotW->currentWidget());
    onStartDraw();
}

APlot::APlot(QWidget *parent):QWidget (parent)
{

}

int APlot::startDraw(const int &lineCount, const int &pointCount)
{
    Q_UNUSED(lineCount)
    Q_UNUSED(pointCount)
    return -1;
}

QtChartPlot::QtChartPlot(QWidget *parent):
    APlot (parent),
    mChart(nullptr),
    mPlotView(new QGraphicsView(new QGraphicsScene, this))
{
    mChart = new QChart;
    mChart->setMinimumSize(640, 480);
    mChart->setTitle("Qt Chart");
    mChart->legend()->hide();
    mChart->createDefaultAxes();

    mPlotView->setRenderHints(QPainter::Antialiasing);
    mPlotView->scene()->addItem(mChart);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(mPlotView);
    setLayout(layout);
    mPlotView->installEventFilter(this);
}

int QtChartPlot::startDraw(const int &lineCount, const int &pointCount)
{
    QTime timeCounter;
    mChart->removeAllSeries();
    timeCounter.start();
    for(int y = 0; y < lineCount; y++) {
        QLineSeries* series = new QLineSeries;
        for(int x = 0; x < pointCount; x++) {
            series->append(x * 2 + 1, y * 2 + 1);
        }
        mChart->addSeries(series);
    }
    mChart->createDefaultAxes();
    return timeCounter.elapsed();
}

bool QtChartPlot::eventFilter(QObject *obj, QEvent *e)
{
    if(e->type() == e->Wheel) {
        e->accept();
        QWheelEvent* we = static_cast<QWheelEvent*>(e);
        if(we->angleDelta().y() > 0) {
            mChart->zoomIn();
        } else{
            mChart->zoomOut();
        }
        return true;
    }
    return QWidget::eventFilter(obj, e);
}

CustumPlot::CustumPlot(QWidget *parent):
    APlot (parent),
    mPlotView(new QCustomPlot(this))
{
    QVBoxLayout* layout = new QVBoxLayout;
    QLabel* pNameLable = new QLabel("QCustom Plot", this);
    layout->addWidget(pNameLable, 0, Qt::AlignHCenter);
    layout->addWidget(mPlotView, 1, Qt::AlignHCenter);
    mPlotView->setMinimumSize(640, 480);
    setLayout(layout);

    mPlotView->installEventFilter(this);

    mPlotView->xAxis->setLabel("x");
    mPlotView->yAxis->setLabel("y");
}

int CustumPlot::startDraw(const int &lineCount, const int &pointCount)
{
    QTime timeCounter;
    mPlotView->clearGraphs();
    timeCounter.start();
    for (int l = 0; l < lineCount; l++) {
        QVector<double> x(pointCount), y(pointCount);
        for(int p = 0; p < pointCount; p++) {
            x[p] = p * 2 + 1;
            y[p] = l * 2 + 1;
        }
        mPlotView->addGraph();
        mPlotView->graph(l)->setData(x, y);
    }

    mPlotView->xAxis->setRange(0, pointCount);
    mPlotView->yAxis->setRange(0, lineCount * 2 + 1);
    mPlotView->replot();
    return timeCounter.elapsed();
}

bool CustumPlot::eventFilter(QObject *obj, QEvent *e)
{
    if(e->type() == e->Wheel) {
        e->accept();
        QWheelEvent* we = static_cast<QWheelEvent*>(e);
        qreal scale = we->angleDelta().y() > 0? 0.9 : 1.1;
        mPlotView->xAxis->setRange(mPlotView->xAxis->range() * scale);
        mPlotView->yAxis->setRange(mPlotView->yAxis->range() * scale);
        mPlotView->replot();
        return true;
    }
    return QWidget::eventFilter(obj, e);
}

AQWtPlot::AQWtPlot(QWidget *parent):
    APlot (parent),
    mPlotView(new QwtPlot(this))
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(mPlotView);
    setLayout(layout);

    mPlotView->setTitle( "QWT Plot" );
    mPlotView->setCanvasBackground( Qt::white );
    mPlotView->setAxisScale( QwtPlot::yLeft, 0.0, 10.0 );
    mPlotView->insertLegend( new QwtLegend() );

    // canvas
    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setLineWidth( 1 );
    canvas->setFrameStyle( QFrame::Box | QFrame::Plain );
    canvas->setBorderRadius( 15 );

    QPalette canvasPalette( Qt::white );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    canvas->setPalette( canvasPalette );

    mPlotView->setCanvas( canvas );

//    curve->setSamples( points );

    // zoom in/out with the wheel
    ( void ) new QwtPlotMagnifier( canvas );
}

int AQWtPlot::startDraw(const int &lineCount, const int &pointCount)
{
    QTime timeCounter;
    foreach(QwtPlotCurve* curve, mplotCurves) {
        curve->detach();
        delete curve;
    }
    mplotCurves.clear();

    timeCounter.start();
    for (int l = 0; l < lineCount; l++) {
        QwtPlotCurve* curve = new QwtPlotCurve;
        QPolygonF line;
        for(int p = 0; p < pointCount; p++) {
            line << QPointF(p * 2 + 1, l * 2 + 1);
        }
        curve->setSamples(line);
        mplotCurves << curve;
        curve->attach(mPlotView);
    }

    mPlotView->replot();
    return timeCounter.elapsed();
}
