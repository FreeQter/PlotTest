#include "unibox.h"

UniBox::UniBox(QObject *parent, const qreal &minX, const qreal &maxX, const qreal &minY, const qreal &maxY, const int& passDirections):
    InteractiveShapeItem ("", parent, ITopLeft|ITop|ITopRight|ILeft|IRight|IBottomLeft|IBottom|IBottomRight)
{
    m_minX = minX;
    m_maxX = maxX;
    m_minY = minY;
    m_maxY = maxY;
    m_passDirections = passDirections;
    initShape();
    initConnect();
}

void UniBox::draw(QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &canvasRect) const
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
    painter->drawLine(pTopLeft, pTopRight);
    painter->drawLine(pBottomLeft, pBottomRight);
    painter->drawLine(pTopLeft, pBottomLeft);
    painter->drawLine(pTopRight, pBottomRight);

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

    QPolygonF arrowLeft;
    arrowLeft << QPointF(0, -5)
            << QPointF(0, 5)
            << QPointF(-8.5, 0)
            << QPointF(0, -5);

    QPolygonF arrowRight;
    arrowRight << QPointF(0, -5)
            << QPointF(0, 5)
            << QPointF(8.5, 0)
            << QPointF(0, -5);

    InteractiveShapeItem::draw(painter, xMap, yMap, canvasRect);\
    QColor arrowColor(255, 100, 45);

    // draw pass directions
    if (m_passDirections & UniDownIn) {
        QPainterPath path;
        path.addPolygon(arrowUp);
        path.translate(pBottom);
        painter->fillPath(path, arrowColor);
    }

    if (m_passDirections & UniDownOut) {
        QPainterPath path;
        path.addPolygon(arrowDown);
        path.translate(pBottom);
        painter->fillPath(path, arrowColor);
    }

    if (m_passDirections & UniUpIn) {
        QPainterPath path;
        path.addPolygon(arrowDown);
        path.translate(pTop);
        painter->fillPath(path, arrowColor);
    }

    if (m_passDirections & UniUpOut) {
        QPainterPath path;
        path.addPolygon(arrowUp);
        path.translate(pTop);
        painter->fillPath(path, arrowColor);
    }

    if (m_passDirections & UniLeftIn) {
        QPainterPath path;
        path.addPolygon(arrowRight);
        path.translate(pLeft);
        painter->fillPath(path, arrowColor);
    }
    if (m_passDirections & UniLeftOut) {
        QPainterPath path;
        path.addPolygon(arrowLeft);
        path.translate(pLeft);
        painter->fillPath(path, arrowColor);
    }

    if (m_passDirections & UniRightIn) {
        QPainterPath path;
        path.addPolygon(arrowLeft);
        path.translate(pRight);
        painter->fillPath(path, arrowColor);
    }
    if (m_passDirections & UniRinghtOut) {
        QPainterPath path;
        path.addPolygon(arrowRight);
        path.translate(pRight);
        painter->fillPath(path, arrowColor);
    }

}

void UniBox::initShape()
{
    QRectF rect;
    rect.setSize(QSizeF(m_maxX - m_minX, m_maxY - m_minY));
    QPainterPath path;
    path.addRect(rect);
    path.translate(m_minX, m_minY);
    setShape(path);
}

void UniBox::initConnect()
{
    connect(this, &InteractiveShapeItem::shapeModified, this, &UniBox::updateDatas);
}

qreal UniBox::minX() const
{
    return m_minX;
}

qreal UniBox::maxX() const
{
    return m_maxX;
}

qreal UniBox::minY() const
{
    return m_minY;
}

qreal UniBox::maxY() const
{
    return m_maxY;
}

int UniBox::passDirections() const
{
    return m_passDirections;
}

void UniBox::setPassDirections(const int &directions)
{
    m_passDirections = directions;
    if(plot()){
        plot()->repaint();
    }
}

void UniBox::setMinX(const qreal &minX)
{
    m_minX = minX;
    initShape();
}

void UniBox::setMaxX(const qreal &maxX)
{
    m_maxX = maxX;
    initShape();
}

void UniBox::setMinY(const qreal &minY)
{
    m_minY = minY;
    initShape();
}

void UniBox::setMaxY(const qreal &maxY)
{
    m_maxY = maxY;
    initShape();
}

void UniBox::updateDatas()
{
    QRectF rect = boundingRect();
    m_minX = rect.x();
    m_minY = rect.y();
    m_maxX = rect.x() + rect.width();
    m_maxY = rect.y() + rect.height();
}
