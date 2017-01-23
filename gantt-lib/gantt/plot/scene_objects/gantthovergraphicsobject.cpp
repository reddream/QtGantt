#include "gantthovergraphicsobject.h"
#include "gantt-lib_global_values.h"
#include "ganttscene.h"

#include <QGraphicsScene>
#include <QPainter>

void GanttHoverGraphicsObject::init()
{
    _item = NULL;
}

GanttHoverGraphicsObject::GanttHoverGraphicsObject(QGraphicsItem* parent)
    : QGraphicsObject(parent)
{
    init();
    m_scene = NULL;
}

GanttHoverGraphicsObject::GanttHoverGraphicsObject(GanttScene *scene, QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    init();
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

void GanttHoverGraphicsObject::setPos(int y)
{
    qreal x = (m_scene)?(m_scene->sceneRect().left()):(0);
    QGraphicsItem::setPos(x,y);
}

void GanttHoverGraphicsObject::setItem(GanttInfoItem *item)
{
    disconnectItem();
    if(!item)
        return;
    _item = item;
    connectItem();
}


void GanttHoverGraphicsObject::setPos(const QPointF &pos)
{
    setPos(pos.y());
}

void GanttHoverGraphicsObject::setPos(qreal /*x*/, qreal y)
{
    setPos(y);
}

void GanttHoverGraphicsObject::onViewResized()
{
    prepareGeometryChange();
}

void GanttHoverGraphicsObject::onItemPosChanged()
{
    if(!_item){
        qCritical("last item not disconnected");
        return;
    }
    setPos(_item->pos());
}

void GanttHoverGraphicsObject::disconnectItem()
{
    if(!_item)
        return;

    _item->disconnect(this);
    _item = NULL;
    setVisible(false); // hide if no item
}

void GanttHoverGraphicsObject::connectItem()
{
    if(!_item)
        return;

    connect(_item,SIGNAL(posChanged()),this,SLOT(onItemPosChanged()));
    connect(_item,SIGNAL(aboutToBeDeleted()),this,SLOT(disconnectItem()));

    setPos(_item->pos());       // show it over _item pos
    setVisible(true);           //
}
