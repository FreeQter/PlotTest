#ifndef LINEX_H
#define LINEX_H

#include "interactiveshapeitem.h"

class Linex: public InteractiveShapeItem
{
    Q_OBJECT
public:
    enum PassDirection{
        LXNone = 0,
        LXUp = 0x00000001,
        LXDown = 0x00000010
    };

    explicit Linex(QObject *parent = nullptr, const qreal &minX = 0,
                   const qreal &maxX = 100, const qreal &y = 100,
                   const int &passDirections = LXDown);
    void draw(QPainter *, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &canvasRect) const override;
    void initShape();
    void initConnect();
    qreal minX() const;
    qreal maxX() const;
    qreal y() const;
    int directions() const;

signals:

public slots:
    void setMinX(const int &minX);
    void setMaxX(const int &maxX);
    void setY(const int &y);
    void updateDatas();
    void setDirections(const int& directions);

private:
    qreal m_minX;
    qreal m_maxX;
    qreal m_y;
    int m_directions;
};

#endif // LINEX_H
