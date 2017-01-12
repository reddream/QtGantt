#ifndef GANTTHOVERGRAPHICSOBJECT_H
#define GANTTHOVERGRAPHICSOBJECT_H

#include <QGraphicsObject>
#include <QPointer>

class GanttScene;

class GanttHoverGraphicsObject : public QGraphicsObject
{
    Q_OBJECT

public:
    GanttHoverGraphicsObject(QGraphicsItem* parent = NULL);

    QRectF boundingRect() const;
    void setScene(GanttScene* scene);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setPos(const QPointF &pos);
    void setPos(qreal x, qreal y);

public slots:
    void onViewResized();

private:
    QPointer<GanttScene> m_scene;

};

#endif // GANTTHOVERGRAPHICSOBJECT_H
