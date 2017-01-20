#include "gantthovergraphicsobject.h"
#include "gantt-lib_global_values.h"
#include "ganttscene.h"

#include <QGraphicsScene>
#include <QPainter>

GanttHoverGraphicsObject::GanttHoverGraphicsObject(QGraphicsItem* parent)
    : QGraphicsObject(parent)
{
    m_scene = NULL;
}

GanttHoverGraphicsObject::GanttHoverGraphicsObject(GanttScene *scene, QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    setScene(scene);
}

QRectF GanttHoverGraphicsObject::boundingRect() const
{
    if(!m_scene)
        return QRectF();
    return QRectF(0,0,m_scene->sceneRect().width(),DEFAULT_ITEM_HEIGHT);
}

void GanttHoverGraphicsObject::setScene(GanttScene *scene)
{
    if(!scene)
        return;
    m_scene = scene;
    prepareGeometryChange();
    scene->addItem(this);
    connect(scene->view(),SIGNAL(viewResized(QSize)),this,SLOT(onViewResized()));
}

void GanttHoverGraphicsObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setOpacity(0.5);
    painter->fillRect(boundingRect(),QColor(0xB1C2E0));

}

void GanttHoverGraphicsObject::setPos(const QPointF &pos)
{
    qreal x = (m_scene)?(m_scene->sceneRect().left()):(0);
    QGraphicsItem::setPos(x,pos.y());
}

void GanttHoverGraphicsObject::setPos(qreal x, qreal y)
{
    setPos(QPointF(x,y));
}

void GanttHoverGraphicsObject::onViewResized()
{
    prepareGeometryChange();
}
