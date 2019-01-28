#ifndef LINEY_H
#define LINEY_H

#include "interactiveshapeitem.h"

class LineY: public InteractiveShapeItem
{
    Q_OBJECT
public:
    enum PassDirection{
        LYNone = 0,
        LYLeft = 0x00000001,
        LYRight = 0x00000010
    };
    explicit LineY(QObject *parent = nullptr, const qreal &minY = 0, const qreal &maxY = 100,
                   const qreal &x = 100, const int &passDirections = LYRight);
    void draw(QPainter *, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &canvasRect) const override;
    void initShape();
    void initConnect();
    qreal minY() const;
    qreal maxY() const;
    qreal x() const;
    int passDirections() const;

public slots:
    void updateDatas();
    void setMinY(const qreal &y);
    void setMaxY(const qreal &y);
    void setX(const qreal &x);
    void setPassDirections(const int &directions);

private:
    qreal m_minY;
    qreal m_maxY;
    qreal m_x;
    int m_passDirections;
};

#endif // LINEY_H
