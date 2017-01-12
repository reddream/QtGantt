#ifndef GANTTGRAPHICSITEM_H
#define GANTTGRAPHICSITEM_H

#include "ganttgraphicsobject.h"
#include "ganttinfoleaf.h"

#include <QPointer>

class GanttIntervalGraphicsObject : public GanttGraphicsObject
{
    Q_OBJECT

public:
    GanttIntervalGraphicsObject(GanttInfoLeaf *info,QGraphicsItem *parent = 0);
    virtual ~GanttIntervalGraphicsObject();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    GanttInfoLeaf *innerInfo() const;


signals:
    void graphicsItemPressed();
    void graphicsItemHoverEnter();
    void graphicsItemHoverLeave();


public slots:
    void setBoundingRectSize(const QSizeF &boundingRectSize);

    void updateItemGeometry();



protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    bool m_isSmall;

    QSizeF m_boundingRectSize;


};

#endif // GANTTGRAPHICSITEM_H
