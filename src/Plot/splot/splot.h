#ifndef SPLOT_H
#define SPLOT_H

#include <QObject>

#include <qwt/qwt_plot.h>
#include <qwt/qwt_scale_draw.h>
#include <qwt/qwt_plot_curve.h>

class QwtPlotCanvas;
class QwtPlotGrid;
class QwtPlotCurve;
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

signals:

public slots:

private:
    SPlotType m_plotType;
    QwtPlotCanvas* m_canvas;
    QwtPlotCurve* m_curve;
};

class SScaleDraw: public QwtScaleDraw{
public:
    explicit SScaleDraw() {}

protected:
    void drawLabel(QPainter* painter, double value) const;
    void drawTick (QPainter *painter, double value, double len) const;
    void drawBackbone (QPainter *painter) const;

};

#endif // SPLOT_H
