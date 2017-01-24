#include "ganttcalcgraphicsobject.h"
#include "gantt-lib_global_values.h"

#include "ganttscene.h"
#include "ganttinfonode.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

GanttCalcGraphicsObject::GanttCalcGraphicsObject(GanttInfoNode* node, QGraphicsItem *parent)
    :GanttGraphicsObject(node,parent)
{
    m_shapePath.moveTo(0,0);
    m_shapePath.lineTo(0,(DEFAULT_ITEM_HEIGHT-6)/2);
    m_shapePath.addEllipse(QPointF(0,(DEFAULT_ITEM_HEIGHT/2)),3,3);
//    m_shapePath.moveTo(0,DEFAULT_ITEM_HEIGHT - (DEFAULT_ITEM_HEIGHT-6)/2);
//    m_shapePath.lineTo(0,DEFAULT_ITEM_HEIGHT);

    if(innerInfo())
    {
        setToolTip( QString::fromUtf8("НУ-РЕШ-ВИТОК:") + '\t' + '\t' + innerInfo()->title()
                    + '\n' + QString::fromUtf8("Время рассчета:") + '\t' + innerInfo()->start().toString("dd.MM.yyyy HH:mm:ss"));

        connect(innerInfo(), SIGNAL(changed()),this,SLOT(updateItemGeometry()));

        innerInfo()->increaseLinkCnt();
    }

    setAcceptHoverEvents(true);

}

GanttCalcGraphicsObject::~GanttCalcGraphicsObject()
{
    if(_scene)
    {
        _scene->removeItem(this);
        setParentItem(NULL);
    }

    if(innerInfo())
        innerInfo()->reduceLinkCnt();
}

QRectF GanttCalcGraphicsObject::boundingRect() const
{
    return m_shapePath.controlPointRect();
}

void GanttCalcGraphicsObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QColor color = (_scene->currentItem() == this)?(QColor(Qt::red).darker(130)):(QColor(Qt::red));
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->drawPath(m_shapePath);
    painter->fillPath(m_shapePath,QBrush(color.lighter(130)));
}

GanttInfoNode *GanttCalcGraphicsObject::innerInfo() const
{
    return qobject_cast<GanttInfoNode*>(m_info);
}

void GanttCalcGraphicsObject::updateItemGeometry(bool checkIntersect)
{
    if(!_scene || !innerInfo())
        return;

    qreal calcPos = _scene->dtToPos(innerInfo()->start());

    setPos(calcPos, innerInfo()->calcPos());

}

void GanttCalcGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void GanttCalcGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    emit graphicsItemHoverEnter();

    QGraphicsItem::hoverEnterEvent(event);
}

void GanttCalcGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    emit graphicsItemHoverLeave();

    QGraphicsItem::hoverLeaveEvent(event);
}


