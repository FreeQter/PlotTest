#include "trianglebox.h"
#include <QtMath>

TriangleBox::TriangleBox(QObject *parent, const qreal &left, const qreal &right, const qreal &top, const qreal &bottom, const int& passDirections):
    InteractiveShapeItem ("", parent)
{
    m_top = top;
    m_bottom = bottom;
    m_left = left;
    m_right = right;
    m_passDirections = passDirections;
    initShape();
    initConnect();
}

void TriangleBox::draw(QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &canvasRect) const
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

    QPainterPath path;
    QPolygonF triangle;
    triangle << QPointF(pBottomLeft)
             << QPointF(pBottomRight)
             << QPointF(pTop)
             << QPointF(pBottomLeft);
    path.addPolygon(triangle);
    painter->drawPath(path);

    InteractiveShapeItem::draw(painter, xMap, yMap, canvasRect);

    QColor arrowColor(255, 100, 45);

    // left line in
    if(m_passDirections & TriangleLeftIn) {
        QPainterPath path;
        path.addPolygon(edgeArrow(pBottomLeft, pTop, -1));
        painter->fillPath(path, arrowColor);
    }

    // left line out
    if(m_passDirections & TriangleLeftOut) {
        QPainterPath path;
        path.addPolygon(edgeArrow(pBottomLeft, pTop, 1));
        painter->fillPath(path, arrowColor);
    }

    // right line in
    if(m_passDirections & TriangleRightIn) {
        QPainterPath path;
        path.addPolygon(edgeArrow(pTop, pBottomRight, -1));
        painter->fillPath(path, arrowColor);
    }

    // right line out
    if(m_passDirections & TriangleRightOut) {
        QPainterPath path;
        path.addPolygon(edgeArrow(pTop, pBottomRight, -1));
        painter->fillPath(path, arrowColor);
    }

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

    if (m_passDirections & TriangleBottomIn) {
        QPainterPath path;
        path.addPolygon(arrowUp);
        path.translate(pBottom);
        painter->fillPath(path, arrowColor);
    }

    if (m_passDirections & TriangleBottomOut) {
        QPainterPath path;
        path.addPolygon(arrowDown);
        path.translate(pBottom);
        painter->fillPath(path, arrowColor);
    }
}

void TriangleBox::initShape()
{
    QRectF rect;
    rect.setSize(QSizeF(m_right - m_left, m_top - m_bottom));
    QPainterPath path;
    path.addRect(rect);
    path.translate(m_left, m_bottom);
    setShape(path);
}

void TriangleBox::initConnect()
{
    connect(this, &InteractiveShapeItem::shapeModified, this, &TriangleBox::updateDatas);
}

qreal TriangleBox::top() const
{
    return m_top;
}

qreal TriangleBox::bottom() const
{
    return m_bottom;
}

qreal TriangleBox::left() const
{
    return m_left;
}

qreal TriangleBox::right() const
{
    return m_right;
}

int TriangleBox::passDirections() const
{
    return m_passDirections;
}

QPolygonF TriangleBox::edgeArrow(const QPointF &p1, const QPointF &p2, const int &xSide) const
{
    qreal br = 5;
    qreal bl = 5 * 1.73; // ~sqrt(3)
    qreal kl = (p2.y() - p1.y()) / (p2.x() - p1.x());
    qreal kkl = 1 / kl;
    QPointF d((p2.x() + p1.x()) / 2, (p2.y() + p1.y()) / 2);
    qreal dd1 = qSqrt(qPow(br, 2) / (kl * kl + 1));
    qreal dd2 = qSqrt(qPow(bl, 2) / (kkl * kkl + 1));
    QPointF c(d.x() - dd1, d.y() - kl * dd1);
    QPointF e(d.x() + dd1, d.y() + kl * dd1);
    QPointF f(d.x() - dd2 * xSide, d.y() + kkl * dd2 * xSide);
    QPolygonF arrow;
    arrow << c << e << f;
    return arrow;
}

void TriangleBox::updateDatas()
{

}

void TriangleBox::setTop(const qreal &top)
{
    m_top = top;
    initShape();
}

void TriangleBox::setBottom(const qreal &bottom)
{
    m_bottom = bottom;
    initShape();
}

void TriangleBox::setLeft(const qreal &left)
{
    m_left = left;
    initShape();
}

void TriangleBox::setRight(const qreal &right)
{
    m_right = right;
    initShape();
}

void TriangleBox::setPassDirections(const int &directions)
{
    m_passDirections = directions;
    if(plot()){
        plot()->repaint();
    }
}
