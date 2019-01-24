#include "interactiveshapeitem.h"

InteractiveShapeItem::InteractiveShapeItem(const QString &title, QObject* parent, const int& interactModes):
    QObject (parent),
    QwtPlotShapeItem (title)

{
    m_interactModes =  interactModes;
}

void InteractiveShapeItem::draw(QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &canvasRect) const
{
    Q_UNUSED(canvasRect)
    QRectF rect = boundingRect();
    painter->setRenderHint(QPainter::Antialiasing);

    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor(40, 150, 200, 60));
    painter->setPen(pen);

    QPointF pTopLeft = rect.bottomLeft();
    pTopLeft = QwtScaleMap::transform(xMap, yMap, pTopLeft);
    QPointF pTopRight = rect.bottomRight();
    pTopRight = QwtScaleMap::transform(xMap, yMap, pTopRight);
    QPointF pBottomLeft = rect.topLeft();
    pBottomLeft = QwtScaleMap::transform(xMap, yMap, pBottomLeft);
    QPointF pBottomRight = rect.topRight();
    pBottomRight = QwtScaleMap::transform(xMap, yMap, pBottomRight);

    painter->drawRect(QRectF(pTopLeft, QSizeF(pTopRight.x() - pTopLeft.x(), qAbs(pTopRight.y() - pBottomRight.y()))));

    QPointF pLeft(pTopLeft.x(), (pTopLeft.y() + pBottomLeft.y()) / 2);
    QPointF pRight(pTopRight.x(), (pTopLeft.y() + pBottomLeft.y()) / 2);
    QPointF pTop((pTopLeft.x() + pTopRight.x()) / 2, pTopLeft.y());
    QPointF pBottom((pTopLeft.x() + pTopRight.x()) / 2, pBottomLeft.y());

    // top
    if(m_interactModes & ITop)
        painter->fillRect(QRectF(pTop - QPointF(10, 0), QSize(20, 20)), QColor(40, 150, 200, 100));
    // Left
    if(m_interactModes & ILeft)
        painter->fillRect(QRectF(pLeft - QPointF(0, 10), QSize(20, 20)), QColor(40, 150, 200, 100));
    // Right
    if(m_interactModes & IRight)
        painter->fillRect(QRectF(pRight - QPointF(20, 10), QSize(20, 20)), QColor(40, 150, 200, 100));
    // Bottom
    if(m_interactModes & IBottom)
        painter->fillRect(QRectF(pBottom - QPointF(10, 20), QSize(20, 20)), QColor(40, 150, 200, 100));
    // TopLeft
    if(m_interactModes & ITopLeft)
        painter->fillRect(QRectF(pTopLeft - QPointF(0, 0), QSize(20, 20)), QColor(40, 150, 200, 100));
    // TopRight
    if(m_interactModes & ITopRight)
        painter->fillRect(QRectF(pTopRight - QPointF(20, 0), QSize(20, 20)), QColor(40, 150, 200, 100));
    // BotomLeft
    if(m_interactModes & IBottomLeft)
        painter->fillRect(QRectF(pBottomLeft - QPointF(0, 20), QSize(20, 20)), QColor(40, 150, 200, 100));
    // BottomRight
    if(m_interactModes & IBottomRight)
        painter->fillRect(QRectF(pBottomRight - QPointF(20, 20), QSize(20, 20)), QColor(40, 150, 200, 100));
}

int InteractiveShapeItem::interactModes() const
{
    return m_interactModes;
}

void InteractiveShapeItem::setInteractModes(const int &modes)
{
    m_interactModes = modes;
}

void InteractiveShapeItem::pressed(const QPoint &pos)
{
    const QwtScaleMap xMap = plot()->canvasMap( xAxis() );
    const QwtScaleMap yMap = plot()->canvasMap( yAxis() );
    qreal xScale = xMap.sDist() / xMap.pDist();
    qreal yScale = yMap.sDist() / yMap.pDist();
    m_currentPos = pos;
    const QPointF p1 = QwtScaleMap::invTransform( xMap, yMap, m_currentPos );
    QRectF rect = boundingRect();
    QPointF pTopLeft = rect.bottomLeft();
    QPointF pTopRight = rect.bottomRight();
    QPointF pBottomLeft = pTopLeft - QPointF(0, rect.height());
    QPointF pBottomRight = pTopRight - QPointF(0, rect.height());
    QPointF pLeft(pTopLeft.x(), (pTopLeft.y() + pBottomLeft.y()) / 2);
    QPointF pRight(pTopRight.x(), (pTopLeft.y() + pBottomLeft.y()) / 2);
    QPointF pTop((pTopLeft.x() + pTopRight.x()) / 2, pTopLeft.y());
    QPointF pBottom((pTopLeft.x() + pTopRight.x()) / 2, pBottomLeft.y());

    const QSizeF size(20 * xScale , 20 * yScale);

    if (containsPos(pTop - QPointF(10 * xScale, 0), size, p1))
        m_dragMode = ITop & m_interactModes;
    else if (containsPos(pLeft - QPointF(0, -10 * yScale), size, p1))
        m_dragMode = ILeft & m_interactModes;
    else if (containsPos(pRight - QPointF(20 * xScale, -10 * yScale), size, p1))
        m_dragMode = IRight & m_interactModes;
    else if (containsPos(pBottom - QPointF(10 * xScale, -20 * yScale), size, p1))
        m_dragMode = IBottom & m_interactModes;
    else if (containsPos(pTopLeft - QPointF(0, 0), size, p1))
        m_dragMode = ITopLeft & m_interactModes;
    else if (containsPos(pTopRight - QPointF(20 * xScale, 0), size, p1))
        m_dragMode = ITopRight & m_interactModes;
    else if (containsPos(pBottomLeft - QPointF(0, -20 * yScale), size, p1))
        m_dragMode = IBottomLeft & m_interactModes;
    else if (containsPos(pBottomRight - QPointF(20 * xScale, -20 * yScale), size, p1))
        m_dragMode = IBottomRight & m_interactModes;
    else
        m_dragMode = INone;
}

void InteractiveShapeItem::move(const QPoint &pos)
{
    const QwtScaleMap xMap = plot()->canvasMap( xAxis() );
    const QwtScaleMap yMap = plot()->canvasMap( yAxis() );

    const QPointF p1 = QwtScaleMap::invTransform( xMap, yMap, m_currentPos );
    const QPointF p2 = QwtScaleMap::invTransform( xMap, yMap, pos );
    m_currentPos = pos;
    QRectF rect = boundingRect();
    QPainterPath path;
    switch (m_dragMode) {
    case INone:{
        path = shape().translated( p2 - p1 );
        setShape(path);
        emit shapeModified();
        break;
    }
    case ITopLeft:{
        qreal dH = p2.y() - p1.y();
        rect.setHeight(rect.height() + dH);
        rect.setX(rect.x() - (p1.x() - p2.x()));
        path.addRect(rect);
        setShape(path);
        emit shapeModified();
        break;
    }
    case ITop:{
        qreal dH = p2.y() - p1.y();
        rect.setHeight(rect.height() + dH);
        path.addRect(rect);
        setShape(path);
        emit shapeModified();
        break;
    }
    case ITopRight:{
        qreal dW = p2.x() - p1.x();
        qreal dH = p2.y() - p1.y();
        rect.setWidth(rect.width() + dW);
        rect.setHeight(rect.height() + dH);
        path.addRect(rect);
        setShape(path);
        emit shapeModified();
        break;
    }

    case ILeft:{
        rect.setX(p2.x());
        path.addRect(rect);
        setShape(path);
        emit shapeModified();
        break;
    }

    case IRight:{
        qreal dW = p2.x() - p1.x();
        rect.setWidth(rect.width() + dW);
        path.addRect(rect);
        setShape(path);
        emit shapeModified();
        break;
    }

    case IBottomLeft:{
        rect.setX(rect.x() - (p1.x() - p2.x()));
        rect.setY(rect.y() - (p1.y() - p2.y()));
        path.addRect(rect);
        setShape(path);
        emit shapeModified();
        break;
    }

    case IBottom:{
        rect.setY(rect.y() - (p1.y() - p2.y()));
        path.addRect(rect);
        setShape(path);
        emit shapeModified();
        break;
    }

    case IBottomRight: {
        qreal dW = p2.x() - p1.x();
        rect.setWidth(rect.width() + dW);
        rect.setY(rect.y() - (p1.y() - p2.y()));
        path.addRect(rect);
        setShape(path);
        emit shapeModified();
        break;
    }
    default:
        break;
    }

}

void InteractiveShapeItem::released(const QPoint &pos)
{
    Q_UNUSED(pos)
    m_dragMode = INone;
}

bool InteractiveShapeItem::containsPos(const QPointF &topLeft, const QSizeF &size, const QPointF &pos)
{
    if(size.isEmpty())
        return false;
    const qreal &x1 = topLeft.x();
    const qreal &y1 = topLeft.y();
    const qreal &w = size.width();
    const qreal &h = size.height();
    const qreal x2 = pos.x();
    const qreal y2 = pos.y();
    return (x2 >= x1 && x2 <= x1 + w && y2 >= y1 - h && y2 <= y1);
}
