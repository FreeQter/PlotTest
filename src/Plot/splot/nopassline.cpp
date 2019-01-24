#include "nopassline.h"

NopassLine::NopassLine(QObject *parent, const qreal &minX, const qreal &maxX, const qreal &y) : InteractiveShapeItem("", parent, ILeft|IRight)
{
    m_minX = minX;
    m_maxX = maxX;
    m_y = y;
    initShape();
    initConnect();
}

void NopassLine::draw(QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &canvasRect) const
{
    painter->setRenderHint(QPainter::Antialiasing);
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

    QPointF pCenter = QPointF((pLeft.x() + pRight.x()) / 2, (pTop.y() + pBottom.y()) / 2);
    QPointF pc1 = QPointF(pCenter.x() - 8, pCenter.y() + 8);
    QPointF pc2 = QPointF(pCenter.x() + 8, pCenter.y() - 8);
    QPointF pc3 = QPointF(pCenter.x() + 8, pCenter.y() + 8);
    QPointF pc4 = QPointF(pCenter.x() - 8, pCenter.y() - 8);

    QColor lineColor(45, 200, 100);
    QPen pen;
    pen.setColor(lineColor);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawLine(pLeft, pRight);
    painter->drawLine(pLeft - QPointF(0, 8), pLeft + QPointF(0, 8));
    painter->drawLine(pRight - QPointF(0, 8), pRight + QPointF(0, 8));
    painter->drawLine(pc1, pc2);
    painter->drawLine(pc3, pc4);
    InteractiveShapeItem::draw(painter, xMap, yMap, canvasRect);
}

void NopassLine::initShape()
{
    QRectF rect;
    rect.setSize(QSizeF(m_maxX - m_minX, 40));
    rect.setX(m_minX);
    QPainterPath path;
    path.addRect(rect);
    path.translate(0, m_y - 20);
    setShape(path);
}

void NopassLine::initConnect()
{
    connect(this, &InteractiveShapeItem::shapeModified, this, &NopassLine::updateDatas);
}

qreal NopassLine::minX() const
{
    return m_minX;
}

qreal NopassLine::maxX() const
{
    return m_maxX;
}

qreal NopassLine::y() const
{
    return m_y;
}

void NopassLine::setMinX(const int &minX)
{
    m_minX = minX;
    initShape();
}

void NopassLine::setMaxX(const int &maxX)
{
    m_maxX = maxX;
    initShape();
}

void NopassLine::setY(const int &y)
{
    m_y = y;
    initShape();
}

void NopassLine::updateDatas()
{
    const QRectF &rect = boundingRect();
    m_minX = rect.x();
    m_maxX = rect.x() + rect.width();
    m_y = rect.y() + 20;
}
