#include "splot.h"

#include <QFile>

#include "qwt_plot_canvas.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_magnifier.h"
#include "qwt_panner.h"
#include "qwt_plot_panner.h"

#include "editor.h"

// test
#include "nopassline.h"
#include "linex.h"
#include "liney.h"
#include "unibox.h"
#include "trianglebox.h"

SPlot::SPlot(QWidget *parent) : QwtPlot (parent)
{
    initData();
    initUI();
    initConnect();

    // test
    NopassLine* noPassLine = new NopassLine;
    noPassLine->attach(this);
    Linex* linex = new Linex;
    linex->attach(this);
    LineY* liney = new LineY;
    liney->attach(this);
    UniBox* box = new UniBox;
    box->attach(this);
    UniBox* box1 = new UniBox;
    box1->attach(this);
    box1->setPassDirections(UniBox::UniUpIn|UniBox::UniRinghtOut);
    TriangleBox* triangle = new TriangleBox;
    triangle->attach(this);

    TriangleBox* triangle1 = new TriangleBox(nullptr, 0, 200, 200, 20, TriangleBox::TriangleBottomIn|TriangleBox::TriangleRightOut);
    triangle1->attach(this);
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
    m_canvas->setMouseTracking(true);

    // default curve
    m_curve = new QwtPlotCurve;
    m_curve->attach(this);

    // editor
    new Editor(this);
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
    m_grid = new QwtPlotGrid;
    m_grid->setPen(QColor(240, 240, 240));
    m_grid->attach(this);
    m_grid->setZ(0);

    // zoom in/out with the wheel
    (void) new QwtPlotMagnifier(m_canvas);

    // colorized scale draw
    setAxisScaleDraw(xBottom, new SScaleDraw);
    setAxisScaleDraw(yLeft, new SScaleDraw);

    // zoom control panel
    m_zoomControlPanel = new ZoomControlFrame(this);
    m_zoomControlPanel->move(m_canvas->x() + 60, m_canvas->y() + 40);

    // style sheet
    QFile f(":/qss/default.qss");
    if(f.open(QIODevice::ReadOnly)) {
        QString qss = f.readAll();
        setStyleSheet(qss);
        f.close();
    }
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


ZoomControlFrame::ZoomControlFrame(QWidget *parent): QFrame (parent)
{
    initData();
    initUI();
    initConnect();
}

void ZoomControlFrame::initData()
{

}

void ZoomControlFrame::initUI()
{
    QHBoxLayout* layout = new QHBoxLayout;
    m_zoomInBtn = new QPushButton(this);
    m_zoomInBtn->setObjectName("ZoomIn");
    m_zoomOutBtn = new QPushButton(this);
    m_zoomOutBtn->setObjectName("ZoomOut");
    m_zoomOriginBtn = new QPushButton(this);
    m_zoomOriginBtn->setObjectName("ZoomOrigin");
    layout->setSpacing(20);
    layout->addWidget(m_zoomInBtn);
    layout->addWidget(m_zoomOutBtn);
    layout->addWidget(m_zoomOriginBtn);
    setLayout(layout);
}

void ZoomControlFrame::initConnect()
{

}
