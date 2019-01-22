#ifndef INTERACTIVESHAPEITEM_H
#define INTERACTIVESHAPEITEM_H

#include <qwt_plot_shapeitem.h>
#include <qwt_plot.h>

class InteractiveShapeItem: public QObject, public QwtPlotShapeItem
{
    Q_OBJECT
    enum InteractMode{
        ITop = 0x00000001,
        IBottom = 0x00000010,
        ILeft = 0x00000100,
        IRight = 0x00001000,
        ITopLeft = 0x00010000,
        ITopRight = 0x00100000,
        IBottomLeft = 0x01000000,
        IBottomRight = 0x10000000
    };

public:
    InteractiveShapeItem(const QString& title = "",
                         QObject *parent = nullptr,
                         const int& interactModes = ITop|IBottom|ILeft|IRight|ITopLeft|ITopRight|IBottomLeft|IBottomRight);
    void attach(QwtPlot *);
    void detach();
    void draw(QPainter *, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &canvasRect) const override;
    int interactModes() const;
    void setInteractModes(const int& modes);

    void pressed(const QPoint &);
    void move(const QPoint &);
    void released(const QPoint &);

protected:
    bool eventFilter(QObject* obj, QEvent* ev) override;

private:
    int m_interactModes;
    QPoint m_currentPos;
    int m_dragMode;
};

#endif // INTERACTIVESHAPEITEM_H
