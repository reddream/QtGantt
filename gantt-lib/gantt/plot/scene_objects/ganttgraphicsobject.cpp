#include "ganttgraphicsobject.h"
#include "gantt/plot/ganttscene.h"
#include "gantt/info/ganttinfoitem.h"


void GanttGraphicsObject::init()
{
    _scene = NULL;
    _dtline = NULL;
}

GanttGraphicsObject::GanttGraphicsObject(GanttInfoItem*item,QGraphicsItem *parent)
    :QGraphicsObject(parent)
{
    m_info = item;
}

GanttGraphicsObject::~GanttGraphicsObject()
{

}

GanttInfoItem *GanttGraphicsObject::info() const
{
    return m_info.data();
}


void GanttGraphicsObject::setScene(GanttScene *scene)
{
    if(!scene)
        return;
    _scene = scene;
    _scene->addItem(this);

}

void GanttGraphicsObject::setDtLine(DtLine *dtline){
    _dtline = dtline;
}

void GanttGraphicsObject::updateItemGeometry(){
    qWarning() << "GanttGraphicsObject::updateItemGeometry using, no override";
}
