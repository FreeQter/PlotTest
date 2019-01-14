#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCharts/QChartGlobal>

class QLineEdit;
class QLabel;
class QStackedWidget;
class QPushButton;
class QButtonGroup;
class APlot;
class QCustomPlot;
class QwtPlot;
class QwtPlotCurve;
class QGraphicsView;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    void initUI();
    void initConnections();
    ~Widget();

public slots:
    void onStartDraw();
    void onPlotSelected(const int& id);

private:
    QLabel* mLineCountLabel;
    QLabel* mPointCountLabel;
    QLineEdit* mLineCountEdit;
    QLineEdit* mPointCountEdit;
    QStackedWidget* mPlotW;
    QPushButton* mDrawBnt;
    QButtonGroup* mPlotsGBnt;
    APlot* mCurrentPlot;
    QLabel* mTimeLabel;
    int mLineCount;
    int mPointCount;
};

class APlot: public QWidget{
    Q_OBJECT
public:
    explicit APlot(QWidget* parent = nullptr);

public slots:
    virtual int startDraw(const int& lineCount = 1, const int& pointCount = 100);
};

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE
QT_CHARTS_USE_NAMESPACE

class QtChartPlot: public APlot{
    Q_OBJECT
public:
    explicit QtChartPlot(QWidget* parent = nullptr);
    int startDraw(const int& lineCount = 1, const int& pointCount = 100) override;

protected:
    bool eventFilter(QObject* obj, QEvent* e) override;

private:
    QChart* mChart;
    QGraphicsView* mPlotView;
};

class CustumPlot: public APlot{
    Q_OBJECT
public:
    explicit CustumPlot(QWidget* parent = nullptr);
    int startDraw(const int& lineCount = 1, const int& pointCount = 100) override;

protected:
    bool eventFilter(QObject* obj, QEvent* e) override;

private:
    QCustomPlot* mPlotView;
};


class AQWtPlot: public APlot{
    Q_OBJECT
public:
    explicit AQWtPlot(QWidget* parent = nullptr);

    int startDraw(const int& lineCount = 1, const int& pointCount = 100) override;

private:
    QwtPlot* mPlotView;
    QList<QwtPlotCurve*> mplotCurves;
};

#endif // WIDGET_H
