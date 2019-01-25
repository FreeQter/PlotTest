#ifndef TRIANGLEBOX_H
#define TRIANGLEBOX_H

#include "interactiveshapeitem.h"

class TriangleBox: public InteractiveShapeItem
{
    Q_OBJECT
public:
    enum PassDirection{
        TriangleNone = 0,
        TriangleLeftIn = 0x00000001,
        TriangleLeftOut = 0x00000010,
        TriangleRightIn = 0x00000100,
        TriangleRightOut = 0x00001000,
        TriangleBottomIn = 0x00010000,
        TriangleBottomOut = 0x00100000
    };
    explicit TriangleBox(QObject *parent = nullptr,
                         const qreal &left = 0, const qreal &right = 100, const qreal &top = 100, const qreal &bottom = 0,
                         const int& passDirections = TriangleLeftIn|TriangleRightOut);
    void draw(QPainter *, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &canvasRect) const override;
    void initShape();
    void initConnect();
    qreal top() const;
    qreal bottom() const;
    qreal left() const;
    qreal right() const;
    int passDirections() const;
    QPolygonF edgeArrow(const QPointF &p1, const QPointF &p2, const int &xSide = -1) const;

public slots:
    void updateDatas();
    void setTop(const qreal &top);
    void setBottom(const qreal &bottom);
    void setLeft(const qreal &left);
    void setRight(const qreal &right);
    void setPassDirections(const int& directions);

private:
    qreal m_top;
    qreal m_bottom;
    qreal m_left;
    qreal m_right;
    int m_passDirections;
};

#endif // TRIANGLEBOX_H
