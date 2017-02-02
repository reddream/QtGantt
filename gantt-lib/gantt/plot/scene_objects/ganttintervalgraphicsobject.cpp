#include "ganttintervalgraphicsobject.h"
#include "gantt/plot/ganttscene.h"
#include "extensions/gantt-lib_global_values.h"

#include "gantt/info/ganttinfonode.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

#define MIN_VIS_WIDTH 8
#define RECTANGLE_OFFSET 5

GanttIntervalGraphicsObject::GanttIntervalGraphicsObject(GanttInfoLeaf *info,QGraphicsItem *parent) :
    GanttGraphicsObject(info,parent)
{
    _scene = NULL;
    _isSmall = true;

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
    if(!_isSmall)
        return QRectF(QPointF(0, RECTANGLE_OFFSET),
                      QSize(_boundingRectSize.width(), _boundingRectSize.height() - 2 * RECTANGLE_OFFSET));
    return QRectF(QPointF( -MIN_VIS_WIDTH/2 - 1, RECTANGLE_OFFSET),
                  QPointF(_boundingRectSize.width()+ MIN_VIS_WIDTH/2 + 1,_boundingRectSize.height() - 2 * RECTANGLE_OFFSET));
}

void GanttIntervalGraphicsObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(!innerInfo() || !_scene)
        return;
    QColor color = (_scene->currentItem() == this)?(innerInfo()->color().darker(130)):(innerInfo()->color());

    if(!_isSmall)
    {
        QRectF drawRect = boundingRect();
        painter->fillRect(drawRect,QBrush(color));
        painter->setPen(QPen(QColor(0x644C26)));
        painter->drawRect(drawRect);

        if(_intersection.isValid())
            painter->fillRect(_intersection,Qt::Dense4Pattern);
    }
    else
    {
        painter->setRenderHint(QPainter::Antialiasing,true);
        QRectF drawRect = QRectF(QPointF(0,0),_boundingRectSize).adjusted(0,5,0,-5);
        painter->fillRect(drawRect,QBrush(color));
        painter->drawRect(drawRect);
        QPainterPath path;

        path.moveTo(-MIN_VIS_WIDTH/2,   drawRect.top());
        path.lineTo(-1,                 drawRect.top() + drawRect.height()/2);
        path.lineTo(-MIN_VIS_WIDTH/2,   drawRect.bottom());
        path.lineTo(-MIN_VIS_WIDTH/2,   drawRect.top());

//        path.moveTo(m_boundingRectSize.width(),
//                                        drawRect.top() + drawRect.height()/2);
        path.moveTo(_boundingRectSize.width() + MIN_VIS_WIDTH/2,
                                        drawRect.top());
        path.lineTo(_boundingRectSize.width() + 1,
                                        drawRect.top() + drawRect.height()/2);
        path.lineTo(_boundingRectSize.width() + MIN_VIS_WIDTH/2,
                                        drawRect.bottom());
        path.lineTo(_boundingRectSize.width() + MIN_VIS_WIDTH/2,
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
        _isSmall = false;
    else
        _isSmall = true;

    _boundingRectSize = boundingRectSize;
}

void GanttIntervalGraphicsObject::updateItemGeometry()
{
    privateUpdateGeometry(false);
}

void GanttIntervalGraphicsObject::updateItemGeometryAndIntersection()
{
    privateUpdateGeometry(true);
}

void GanttIntervalGraphicsObject::updateIntersection()
{
    QList<QGraphicsItem*> collItems = collidingItems();

    QRectF res, rect = boundingRect();
    foreach(QGraphicsItem *item, collItems){
        if(!dynamic_cast<GanttIntervalGraphicsObject*>(item))
            continue;
        res |= rect.intersect( mapRectFromItem(item,item->boundingRect()));
    }
    _intersection = res;
    _lastBoundingRectSize = res.size();

    update();
}

void GanttIntervalGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug() << "GanttIntervalGraphicsObject";
//    updateIntersection();
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

void GanttIntervalGraphicsObject::privateUpdateGeometry(bool intersection)
{
    if(!innerInfo())
        return;

    qreal startPos = _dtline->dtToPos(innerInfo()->start()),
          itemWidth = _dtline->dtToPos(innerInfo()->finish()) - startPos;

    if(intersection){   // UPDATE Intersection
        if(!_intersection.isEmpty()){
            qreal lastLeft = _intersection.left(),
                    lastWidth = _intersection.width(),
                    coef = itemWidth / _lastBoundingRectSize.width();
            _intersection.setLeft( lastLeft * coef );
            _intersection.setWidth( lastWidth * coef );

            _lastBoundingRectSize = _intersection.size();
        }
    }
    // UPDATE geometry
    setBoundingRectSize(QSizeF(itemWidth, DEFAULT_ITEM_WIDTH));
    setPos(startPos, innerInfo()->calcPos());
}





