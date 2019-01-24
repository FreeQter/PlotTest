#ifndef LINEY_H
#define LINEY_H

#include "interactiveshapeitem.h"

class LineY: public InteractiveShapeItem
{
    Q_OBJECT
public:
    explicit LineY(QObject *parent = nullptr, const qreal &minY = 0, const qreal &maxY = 100, const qreal &x = 100);
    void draw(QPainter *, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &canvasRect) const override;
    void initShape();
    void initConnect();
    qreal minY() const;
    qreal maxY() const;
    qreal x() const;

public slots:
    void updateDatas();
    void setMinY(const qreal &y);
    void setMaxY(const qreal &y);
    void setX(const qreal &x);

private:
    qreal m_minY;
    qreal m_maxY;
    qreal m_x;
};

#endif // LINEY_H
