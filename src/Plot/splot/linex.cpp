#include "linex.h"

Linex::Linex(QObject *parent, const qreal &minX, const qreal &maxX, const qreal &y, const int &passDirections):
    InteractiveShapeItem("", parent, ILeft|IRight)
{
    m_minX = minX;
    m_maxX = maxX;
    m_y = y;
    m_directions = passDirections;
    initShape();
    initConnect();
}

void Linex::draw(QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &canvasRect) const
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

    QColor lineColor(45, 200, 100);
    QPen pen;
    pen.setColor(lineColor);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawLine(pLeft, pRight);
    painter->drawLine(pLeft - QPointF(0, 8), pLeft + QPointF(0, 8));
    painter->drawLine(pRight - QPointF(0, 8), pRight + QPointF(0, 8)); 
    InteractiveShapeItem::draw(painter, xMap, yMap, canvasRect);

    QColor arrowColor(255, 100, 45);
    QPolygonF arrowUp;
    arrowUp << QPointF(-5, 0)
            << QPointF(5, 0)
            << QPointF(0, -8.5)
            << QPointF(-5, 0);

    QPolygonF arrowDown;
    arrowDown << QPointF(-5, 0)
            << QPointF(5, 0)
            << QPointF(0, 8.5)
            << QPointF(-5, 0);

    if(m_directions & LXUp) {
        QPainterPath path;
        path.addPolygon(arrowUp);
        path.translate(pTop.x(), pLeft.y());
        painter->fillPath(path, arrowColor);
    }

    if(m_directions & LXDown) {
        QPainterPath path;
        path.addPolygon(arrowDown);
        path.translate(pTop.x(), pLeft.y());
        painter->fillPath(path, arrowColor);
    }
}

void Linex::initShape()
{
    QRectF rect;
    rect.setSize(QSizeF(m_maxX - m_minX, 40));
    rect.setX(m_minX);
    QPainterPath path;
    path.addRect(rect);
    path.translate(0, m_y - 20);
    setShape(path);
}

void Linex::initConnect()
{
    connect(this, &InteractiveShapeItem::shapeModified, this, &Linex::updateDatas);
}

qreal Linex::minX() const
{
    return m_minX;
}

qreal Linex::maxX() const
{
    return m_maxX;
}

qreal Linex::y() const
{
    return m_y;
}

int Linex::directions() const
{
    return m_directions;
}

void Linex::setMinX(const int &minX)
{
    m_minX = minX;
    initShape();
}

void Linex::setMaxX(const int &maxX)
{
    m_maxX = maxX;
    initShape();
}

void Linex::setY(const int &y)
{
    m_y = y;
    initShape();
}

void Linex::updateDatas()
{
    const QRectF &rect = boundingRect();
    m_minX = rect.x();
    m_maxX = rect.x() + rect.width();
    m_y = rect.y() + 20;
}

void Linex::setDirections(const int &directions)
{
    m_directions = directions;
    if(plot()) {
        plot()->update();
    }
 }
