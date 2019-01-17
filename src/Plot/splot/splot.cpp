#include "splot.h"

#include <qwt/qwt_plot_canvas.h>
#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_plot_magnifier.h>
#include <qwt/qwt_panner.h>

SPlot::SPlot(QWidget *parent) : QwtPlot (parent)
{
    initData();
    initUI();
    initConnect();
}

void SPlot::initData()
{
    this->m_plotType = YxPlot;
//    setAxisTitle(Axis::yLeft, "Pa");
//    setAxisTitle(Axis::xBottom, "mm");

    m_canvas = new QwtPlotCanvas;
    setCanvas(m_canvas);
}

void SPlot::initUI()
{
    // plot root
    QPalette widgetPalette(Qt::white);
    widgetPalette.setColor(QPalette::Background, Qt::white);
    setPalette(widgetPalette);

    // canvas
    setCanvasBackground(Qt::white);
    m_canvas->setFrameStyle( QFrame::Box | QFrame::Plain );
    QPalette canvasPalette( Qt::white );
    canvasPalette.setColor( QPalette::Foreground, Qt::white);
    canvasPalette.setColor( QPalette::Background, Qt::white);
    m_canvas->setPalette( canvasPalette );

    // grid
    QwtPlotGrid* grid = new QwtPlotGrid;
    grid->setPen(QColor(200, 200, 200));
    grid->attach(this);

    // zoom in/out with the wheel
    ( void ) new QwtPlotMagnifier( m_canvas );

    // colorized scale draw
    setAxisScaleDraw(xBottom, new SScaleDraw);
    setAxisScaleDraw(yLeft, new SScaleDraw);

    // panner
    (void) new QwtPanner(m_canvas);

}

void SPlot::initConnect()
{

}

SScaleDraw::SScaleDraw(): QwtScaleDraw ()
{

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

