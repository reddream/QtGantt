#include "ganttgraphicsobject.h"
#include "ganttscene.h"
#include "ganttinfoitem.h"


GanttGraphicsObject::GanttGraphicsObject(GanttInfoItem*item,QGraphicsItem *parent)
    :QGraphicsObject(parent)
{
    m_info = item;
}

GanttInfoItem *GanttGraphicsObject::info() const
{
    return m_info.data();
}


void GanttGraphicsObject::setScene(GanttScene *scene)
{
    if(!scene)
        return;
    m_scene = scene;
    m_scene->addItem(this);
}
