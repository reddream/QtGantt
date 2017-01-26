#ifndef GANTTCALCGRAPHICSITEM_H
#define GANTTCALCGRAPHICSITEM_H

#include "ganttgraphicsobject.h"
#include <QPainterPath>

#include <QPointer>

class GanttHeader;
class GanttScene;
class GanttInfoNode;

class GanttCalcGraphicsObject : public GanttGraphicsObject
{
    Q_OBJECT

public:
    GanttCalcGraphicsObject(GanttInfoNode* node,QGraphicsItem *parent = NULL);
    ~GanttCalcGraphicsObject();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    GanttInfoNode *innerInfo() const;

public slots:
    void updateItemGeometry();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);


private:
    QPainterPath m_shapePath;
};

#endif // GANTTCALCGRAPHICSITEM_H
