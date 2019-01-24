#ifndef UNIBOX_H
#define UNIBOX_H

#include "interactiveshapeitem.h"

class UniBox: public InteractiveShapeItem
{
    Q_OBJECT
public:
    enum PassDirection{
        UniNone = 0,
        UniUpIn = 0x00000001,
        UniUpOut = 0x00000010,
        UniRightIn = 0x00000100,
        UniRinghtOut = 0x00001000,
        UniDownIn = 0x00010000,
        UniDownOut = 0x00100000,
        UniLeftIn = 0x01000000,
        UniLeftOut = 0x10000000
    };
    explicit UniBox(QObject *parent = nullptr, const qreal &minX = 0,
                    const qreal &maxX = 100, const qreal &minY = 0,
                    const qreal &maxY = 100, const int& passDirections = UniLeftIn|UniDownOut);
    void draw(QPainter *, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &canvasRect) const override;
    void initShape();
    void initConnect();
    qreal minX() const;
    qreal maxX() const;
    qreal minY() const;
    qreal maxY() const;
    int passDirections() const;
    void setPassDirections(const int& directions);

public slots:
    void setMinX(const qreal &minX);
    void setMaxX(const qreal &maxX);
    void setMinY(const qreal &minY);
    void setMaxY(const qreal &maxY);

    void updateDatas();

private:
    qreal m_minX;
    qreal m_maxX;
    qreal m_minY;
    qreal m_maxY;
    int m_passDirections;
};

#endif // UNIBOX_H
