#include "splot.h"

#include <QtWidgets>

#include "qwt_plot_canvas.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_magnifier.h"
#include "qwt_panner.h"

SPlot::SPlot(QWidget *parent) : QwtPlot (parent)
{
    initData();
    initUI();
    initConnect();
}

void SPlot::initData()
{
    this->m_plotType = YxPlot;
    m_titleText.setColor(QColor(120, 120, 120));
    QFont font;
    font.setBold(true);
    font.setPointSize(12);
    m_titleText.setFont(font);
    m_titleText.setText("Y(x)");
    setTitle(m_titleText);

    // canvas
    m_canvas = new QwtPlotCanvas;
    setCanvas(m_canvas);

    // default curve
    m_curve = new QwtPlotCurve;
    m_curve->attach(this);
}

void SPlot::initUI()
{
    // plot root
    QPalette widgetPalette(Qt::white);
    widgetPalette.setColor(QPalette::Background, Qt::white);
    setPalette(widgetPalette);

    // canvas
    setCanvasBackground(Qt::white);
    m_canvas->setFrameStyle(QFrame::Box|QFrame::Plain);
    QPalette canvasPalette(Qt::white);
    canvasPalette.setColor(QPalette::Foreground, Qt::white);
    canvasPalette.setColor(QPalette::Background, Qt::white);
    m_canvas->setPalette(canvasPalette);

    // grid
    QwtPlotGrid* grid = new QwtPlotGrid;
    grid->setPen(QColor(240, 240, 240));
    grid->attach(this);

    // zoom in/out with the wheel
    (void) new QwtPlotMagnifier(m_canvas);

    // colorized scale draw
    setAxisScaleDraw(xBottom, new SScaleDraw);
    setAxisScaleDraw(yLeft, new SScaleDraw);

    // panner
    (void) new QwtPanner(m_canvas);
}

void SPlot::initConnect()
{

}

void SPlot::setSamples(const QVector<QPointF> datas)
{
    m_curve->setSamples(datas);
    replot();
}

void SScaleDraw::drawLabel(QPainter *painter, double value) const
{
    painter->setPen(QColor(100, 100, 100));
    QwtScaleDraw::drawLabel(painter, value);
}

void SScaleDraw::drawTick(QPainter *painter, double value, double len) const
{
    painter->setPen(QColor(150, 150, 150));
    QwtScaleDraw::drawTick(painter, value, len);
}

void SScaleDraw::drawBackbone(QPainter *painter) const
{
    painter->setPen(QColor(150, 150, 150));
    QwtScaleDraw::drawBackbone(painter);
}

