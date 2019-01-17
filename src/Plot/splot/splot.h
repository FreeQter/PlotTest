#ifndef SPLOT_H
#define SPLOT_H

#include <QObject>

#include <qwt/qwt_plot.h>
#include <qwt/qwt_scale_draw.h>

class QwtPlotCanvas;
class QwtPlotGrid;
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

signals:

public slots:

private:
    SPlotType m_plotType;
    QwtPlotCanvas* m_canvas;
};

class SScaleDraw: public QwtScaleDraw{
public:
    explicit SScaleDraw();

protected:
    void drawLabel(QPainter* painter, double value) const;
    void drawTick (QPainter *painter, double value, double len) const;
    void drawBackbone (QPainter *painter) const;

};

#endif // SPLOT_H
