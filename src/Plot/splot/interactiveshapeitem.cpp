#include "interactiveshapeitem.h"

InteractiveShapeItem::InteractiveShapeItem(const QString &title, QObject* parent, const int& interactModes):
    QObject (parent),
    QwtPlotShapeItem (title)

{
    m_interactModes =  interactModes;
}

void InteractiveShapeItem::attach(QwtPlot *plot)
{
    QwtPlotShapeItem::attach(plot);
    plot->canvas()->installEventFilter(this);
}

void InteractiveShapeItem::detach()
{
    QwtPlotShapeItem::detach();
    this->deleteLater();
}

void InteractiveShapeItem::draw(QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &canvasRect) const
{
    Q_UNUSED(canvasRect)
    QRectF rect = boundingRect();

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
//    painter->drawRect(QRectF(pTopLeft, QSizeF(pTopRight.x() - pTopLeft.x(), qAbs(pTopRight.y() - pBottomRight.y()))));

    QPointF pLeft(pTopLeft.x(), (pTopLeft.y() + pBottomLeft.y()) / 2);
    QPointF pRight(pTopRight.x(), (pTopLeft.y() + pBottomLeft.y()) / 2);
    QPointF pTop((pTopLeft.x() + pTopRight.x()) / 2, pTopLeft.y());
    QPointF pBottom((pTopLeft.x() + pTopRight.x()) / 2, pBottomLeft.y());

//    m_rtop = QRectF(pTop - QPointF(10, 0), QSize(20, 20));

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
    m_currentPos = pos;
    const QPointF p1 = QwtScaleMap::invTransform( xMap, yMap, m_currentPos );
    QRectF rect = boundingRect();
    QPointF pTopLeft = rect.bottomLeft();
    QPointF pTopRight = rect.bottomRight();
    QPointF pBottomLeft = rect.topLeft();
    QPointF pBottomRight = rect.topRight();
//    pTopLeft = QwtScaleMap::transform(xMap, yMap, pTopLeft);
//    pTopRight = QwtScaleMap::transform(xMap, yMap, pTopRight);
//    pBottomLeft = QwtScaleMap::transform(xMap, yMap, pBottomLeft);
//    pBottomRight = QwtScaleMap::transform(xMap, yMap, pBottomRight);
    QPointF pLeft(pTopLeft.x(), (pTopLeft.y() + pBottomLeft.y()) / 2);
    QPointF pRight(pTopRight.x(), (pTopLeft.y() + pBottomLeft.y()) / 2);
    QPointF pTop((pTopLeft.x() + pTopRight.x()) / 2, pTopLeft.y());
    QPointF pBottom((pTopLeft.x() + pTopRight.x()) / 2, pBottomLeft.y());

    QRectF rTop(pTop - QPointF(10, 0), QSize(20, -20));
    QRectF rLeft(pLeft - QPointF(0, 10), QSize(20, -20));
    QRectF rRight(pRight - QPointF(20, 10), QSize(20, -20));
    QRectF rBottom(pBottom - QPointF(10, 20), QSize(20, -20));
    QRectF rTopLeft(pTopLeft - QPointF(0, 0), QSize(20, -20));
    QRectF rTopRight(pTopRight - QPointF(20, 0), QSize(20, -20));
    QRectF rBottomLeft(pBottomLeft - QPointF(0, 20), QSize(20, -20));
    QRectF rBottomRight(pBottomRight - QPointF(20, 20), QSize(20, -20));

    if (rTop.contains(p1))
        m_dragMode = ITop;
    else if (rLeft.contains(p1))
        m_dragMode = ILeft;
    else if (rRight.contains(p1))
        m_dragMode = IRight;
    else if (rBottom.contains(p1))
        m_dragMode = IBottom;
    else if (rTopLeft.contains(p1))
        m_dragMode = ITopLeft;
    else if (rTopRight.contains(p1))
        m_dragMode = ITopRight;
    else if (rBottomLeft.contains(p1))
        m_dragMode = IBottomLeft;
    else if (rBottomRight.contains(p1))
        m_dragMode = IBottomRight;
    else
        m_dragMode = 0;
    qDebug () << rTop.bottomRight() << p1 << boundingRect() << rTop.contains(p1) << m_dragMode;
}

void InteractiveShapeItem::move(const QPoint &pos)
{
    //    const QwtScaleMap xMap = plot()->canvasMap( d_editedItem->xAxis() );
    //    const QwtScaleMap yMap = plot()->canvasMap( d_editedItem->yAxis() );

    //    const QPointF p1 = QwtScaleMap::invTransform( xMap, yMap, d_currentPos );
    //    const QPointF p2 = QwtScaleMap::invTransform( xMap, yMap, pos );

    //#if QT_VERSION >= 0x040600
    //    const QPainterPath shape = d_editedItem->shape().translated( p2 - p1 );
    //#else
    //    const double dx = p2.x() - p1.x();
    //    const double dy = p2.y() - p1.y();

    //    QPainterPath shape = d_editedItem->shape();
    //    for ( int i = 0; i < shape.elementCount(); i++ )
    //    {
    //        const QPainterPath::Element &el = shape.elementAt( i );
    //        shape.setElementPositionAt( i, el.x + dx, el.y + dy );
    //    }
    //#endif

    //    d_editedItem->setShape( shape );
    switch (m_dragMode) {
    case 0:{
        const QwtScaleMap xMap = plot()->canvasMap( xAxis() );
        const QwtScaleMap yMap = plot()->canvasMap( yAxis() );
        const QPointF p1 = QwtScaleMap::invTransform( xMap, yMap, m_currentPos );
        const QPointF p2 = QwtScaleMap::invTransform( xMap, yMap, pos );
        const QPainterPath path = shape().translated( p2 - p1 );
        setShape(path);
        m_currentPos = pos;
        break;
    }

    }

}

void InteractiveShapeItem::released(const QPoint &pos)
{

}

bool InteractiveShapeItem::eventFilter(QObject *obj, QEvent *ev)
{

    return QObject::eventFilter(obj, ev);
}
