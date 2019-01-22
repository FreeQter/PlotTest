#ifndef SPLOT_H
#define SPLOT_H

#include <QObject>
#include <QtWidgets>

#include "qwt_plot.h"
#include "qwt_scale_draw.h"
#include "qwt_plot_curve.h"
#include "qwt_text.h"

class QwtPlotCanvas;
class QwtPlotGrid;
class QwtPlotCurve;
class ZoomControlFrame;

class SPlot : public QwtPlot
{
    Q_OBJECT
    enum SPlotType{
        YxPlot, // (Pa, mm)
        YtPlot, // (Pa, ms)
        XtPlot  // (mm, ms)
    };

public:
    explicit SPlot(QWidget *parent = nullptr);
    void initData();
    void initUI();
    void initConnect();

    void setSamples(const QVector<QPointF> datas);

protected:

signals:

public slots:

private:
    SPlotType m_plotType;
    QwtPlotCanvas* m_canvas;
    QwtPlotCurve* m_curve;
    QwtText m_titleText;
    ZoomControlFrame* m_zoomControlPanel;
};

class SScaleDraw: public QwtScaleDraw{
public:
    explicit SScaleDraw() {}

protected:
    void drawLabel(QPainter* painter, double value) const;
    void drawTick (QPainter *painter, double value, double len) const;
    void drawBackbone (QPainter *painter) const;
};


class ZoomControlFrame: public QFrame{
    Q_OBJECT
public:
    explicit ZoomControlFrame(QWidget* parent = nullptr);
    void initData();
    void initUI();
    void initConnect();

private:
    QPushButton* m_zoomInBtn;
    QPushButton* m_zoomOutBtn;
    QPushButton* m_zoomOriginBtn;
};

#endif // SPLOT_H
