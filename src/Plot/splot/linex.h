#ifndef LINEX_H
#define LINEX_H

#include "interactiveshapeitem.h"

class Linex: public InteractiveShapeItem
{
    Q_OBJECT
public:
    explicit Linex(QObject *parent = nullptr, const qreal &minX = 0, const qreal &maxX = 100, const qreal &y = 100);
    void draw(QPainter *, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &canvasRect) const override;
    void initShape();
    void initConnect();
    qreal minX() const;
    qreal maxX() const;
    qreal y() const;

signals:

public slots:
    void setMinX(const int &minX);
    void setMaxX(const int &maxX);
    void setY(const int &y);
    void updateDatas();

private:
    qreal m_minX;
    qreal m_maxX;
    qreal m_y;
};

#endif // LINEX_H
