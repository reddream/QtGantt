#include "ganttintervalgraphicsobject.h"
#include "ganttscene.h"
#include "gantt-lib_global_values.h"

#include "ganttinfonode.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

#define MIN_VIS_WIDTH 8

GanttIntervalGraphicsObject::GanttIntervalGraphicsObject(GanttInfoLeaf *info,QGraphicsItem *parent) :
    GanttGraphicsObject(info,parent)
{
    _scene = NULL;
    m_isSmall = true;

    if(innerInfo())
    {
        setToolTip( QString::fromUtf8("Операция:") + '\t' + '\t' + innerInfo()->title()
                    + '\n' + QString::fromUtf8("Начало:") + '\t' + innerInfo()->start().toString("dd.MM.yyyy HH:mm:ss")
                    + '\n' + QString::fromUtf8("Окончание:") + '\t' + innerInfo()->finish().toString("dd.MM.yyyy HH:mm:ss"));
        connect(innerInfo(), SIGNAL(changed()),this,SLOT(updateItemGeometry()));

        innerInfo()->increaseLinkCnt();
    }

    setAcceptHoverEvents(true);
}

GanttIntervalGraphicsObject::~GanttIntervalGraphicsObject()
{

    if(_scene)
    {
        _scene->removeItem(this);
        setParentItem(NULL);
    }

    if(innerInfo())
        innerInfo()->reduceLinkCnt();
}

QRectF GanttIntervalGraphicsObject::boundingRect() const
{
    if(!m_isSmall)
        return QRectF(QPointF(0,0),m_boundingRectSize);
    return QRectF(QPointF(-MIN_VIS_WIDTH/2 - 1,0),
                  QPointF(m_boundingRectSize.width()+MIN_VIS_WIDTH/2 + 1,m_boundingRectSize.height()));
}

void GanttIntervalGraphicsObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(!innerInfo() || !_scene)
        return;
    QColor color = (_scene->currentItem() == this)?(innerInfo()->color().darker(130)):(innerInfo()->color());

    if(!m_isSmall)
    {
        QRectF drawRect = boundingRect().adjusted(0,5,0,-5);
        painter->fillRect(drawRect,QBrush(color));
        painter->drawRect(drawRect);
    }
    else
    {
        painter->setRenderHint(QPainter::Antialiasing,true);
        QRectF drawRect = QRectF(QPointF(0,0),m_boundingRectSize).adjusted(0,5,0,-5);
        painter->fillRect(drawRect,QBrush(color));
        painter->drawRect(drawRect);
        QPainterPath path;

        path.moveTo(-MIN_VIS_WIDTH/2,   drawRect.top());
        path.lineTo(-1,                 drawRect.top() + drawRect.height()/2);
        path.lineTo(-MIN_VIS_WIDTH/2,   drawRect.bottom());
        path.lineTo(-MIN_VIS_WIDTH/2,   drawRect.top());

//        path.moveTo(m_boundingRectSize.width(),
//                                        drawRect.top() + drawRect.height()/2);
        path.moveTo(m_boundingRectSize.width() + MIN_VIS_WIDTH/2,
                                        drawRect.top());
        path.lineTo(m_boundingRectSize.width() + 1,
                                        drawRect.top() + drawRect.height()/2);
        path.lineTo(m_boundingRectSize.width() + MIN_VIS_WIDTH/2,
                                        drawRect.bottom());
        path.lineTo(m_boundingRectSize.width() + MIN_VIS_WIDTH/2,
                    drawRect.top());

        painter->setOpacity(0.5);
        painter->fillPath(path,QBrush(Qt::black));
        painter->setOpacity(1);

    }
}

GanttInfoLeaf *GanttIntervalGraphicsObject::innerInfo() const
{
    return qobject_cast<GanttInfoLeaf*>(m_info);
}


void GanttIntervalGraphicsObject::setBoundingRectSize(const QSizeF &boundingRectSize)
{
    prepareGeometryChange();
    if(boundingRectSize.width()>MIN_VIS_WIDTH)
        m_isSmall = false;
    else
        m_isSmall = true;

    m_boundingRectSize = boundingRectSize;
}

void GanttIntervalGraphicsObject::updateItemGeometry()
{
    if( !innerInfo())
        return;

    qreal startPos = _dtline->dtToPos(innerInfo()->start()),
          itemWidth = _dtline->dtToPos(innerInfo()->finish()) - startPos;

    setBoundingRectSize(QSizeF(itemWidth, DEFAULT_ITEM_WIDTH));
    setPos(startPos, innerInfo()->pos());
}

void GanttIntervalGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    if(!m_scene || !innerInfo())
//        return;

//    if(innerInfo()->parent() && (this == m_scene->itemAt(event->scenePos())))
//        if((!innerInfo()->parent()->isExpanded() && (event->button() == Qt::LeftButton))
//                /*|| (innerInfo()->parent()->isExpanded() && (event->button() == Qt::RightButton))*/)
//        {
//            m_scene->changeExpanding(innerInfo()->parent()->index());
//            emit graphicsItemPressed();
//        }

    QGraphicsItem::mousePressEvent(event);
}

void GanttIntervalGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    emit graphicsItemHoverEnter();

    QGraphicsItem::hoverEnterEvent(event);
}

void GanttIntervalGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    emit graphicsItemHoverLeave();

    QGraphicsItem::hoverLeaveEvent(event);
}





