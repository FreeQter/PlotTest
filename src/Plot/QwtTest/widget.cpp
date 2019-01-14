#include "widget.h"

#include <QDebug>
#include <QTime>

#include "qwt_plot_curve.h"
#include "qwt_plot_grid.h"
#include "qwt_symbol.h"
#include "qwt_legend.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_magnifier.h"

static QTime timeCounter;
Widget::Widget(QWidget *parent)
    : QwtPlot(parent)
{

    timeCounter.start();
    this->setTitle( "Plot Demo" );
    this->setCanvasBackground( Qt::white );
    this->setAxisScale( QwtPlot::yLeft, 0.0, 10.0 );
    this->insertLegend( new QwtLegend() );

    // axes
//    setAxisTitle( xBottom, "x -->" );
//    setAxisScale( xBottom, 0.0, 10.0 );

//    setAxisTitle( yLeft, "y -->" );
//    setAxisScale( yLeft, -1.0, 1.0 );

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->attach( this );

    QwtPlotCurve *curve = new QwtPlotCurve();
    curve->setTitle( "Some Points" );
    curve->setPen( Qt::blue, 4 );
    curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );

    QwtSymbol *symbol = new QwtSymbol( QwtSymbol::Ellipse,
        QBrush( Qt::yellow ), QPen( Qt::red, 2 ), QSize( 8, 8 ) );
    curve->setSymbol( symbol );

    QPolygonF points;
    for (int i = 0; i < 8000; i++) {
        qreal x = (i * 3) + 1;
        qreal y = 5;
        points << QPointF(x, y);
    }

    // canvas
    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setLineWidth( 1 );
    canvas->setFrameStyle( QFrame::Box | QFrame::Plain );
    canvas->setBorderRadius( 15 );

    QPalette canvasPalette( Qt::white );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    canvas->setPalette( canvasPalette );

    setCanvas( canvas );

    curve->setSamples( points );

    // zoom in/out with the wheel
    ( void ) new QwtPlotMagnifier( canvas );

    curve->attach( this );
    qDebug() << "init 8000 points: " << timeCounter.elapsed() << "ms";
}

Widget::~Widget()
{

}
