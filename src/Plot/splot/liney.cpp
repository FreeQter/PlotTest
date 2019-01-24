#include "liney.h"

LineY::LineY(QObject *parent, const qreal &minY, const qreal &maxY, const qreal &x):InteractiveShapeItem ("", parent, ITop | IBottom)
{
    m_minY = minY;
    m_maxY = maxY;
    m_x = x;
    initShape();
    initConnect();
}

void LineY::draw(QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &canvasRect) const
{
    QRectF rect = boundingRect();
    QPointF pTopLeft = rect.bottomLeft();
    pTopLeft = QwtScaleMap::transform(xMap, yMap, pTopLeft);
    QPointF pTopRight = rect.bottomRight();
    pTopRight = QwtScaleMap::transform(xMap, yMap, pTopRight);
    QPointF pBottomLeft = rect.topLeft();
    pBottomLeft = QwtScaleMap::transform(xMap, yMap, pBottomLeft);
    QPointF pBottomRight = rect.topRight();
    pBottomRight = QwtScaleMap::transform(xMap, yMap, pBottomRight);

    QPointF pLeft(pTopLeft.x(), (pTopLeft.y() + pBottomLeft.y()) / 2);
    QPointF pRight(pTopRight.x(), (pTopLeft.y() + pBottomLeft.y()) / 2);
    QPointF pTop((pTopLeft.x() + pTopRight.x()) / 2, pTopLeft.y());
    QPointF pBottom((pTopLeft.x() + pTopRight.x()) / 2, pBottomLeft.y());

    QColor lineColor(45, 200, 100);
    QPen pen;
    pen.setColor(lineColor);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawLine(pTop, pBottom);
    painter->drawLine(pTop - QPointF(8, 0), pTop + QPointF(8, 0));
    painter->drawLine(pBottom - QPointF(8, 0), pBottom + QPointF(8, 0));
    InteractiveShapeItem::draw(painter, xMap, yMap, canvasRect);
}

void LineY::initShape()
{
    QRectF rect;
    rect.setSize(QSizeF(40, m_maxY - m_minY));
    QPainterPath path;
    path.addRect(rect);
    path.translate(m_x - 20, m_minY);
    setShape(path);
}

void LineY::initConnect()
{
    connect(this, &InteractiveShapeItem::shapeModified, this, &LineY::updateDatas);
}

qreal LineY::minY() const
{
    return m_minY;
}

qreal LineY::maxY() const
{
    return m_maxY;
}

qreal LineY::x() const
{
    return m_x;
}

void LineY::updateDatas()
{
    const QRectF &rect = boundingRect();
    m_minY = rect.y();
    m_maxY = rect.y() + rect.height();
    m_x = rect.x() + 20;
}

void LineY::setMinY(const qreal &y)
{
    m_minY = y;
    initShape();
}

void LineY::setMaxY(const qreal &y)
{
    m_maxY = y;
    initShape();
}

void LineY::setX(const qreal &x)
{
    m_x = x;
    initShape();
}
