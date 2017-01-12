#include "ganttcurrentdtslider.h"
#include "gantt-lib_global_values.h"
#include "ganttscene.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QPair>

#include <QDebug>

GanttCurrentDtSlider::GanttCurrentDtSlider(QGraphicsItem* parent) :
    QGraphicsObject(parent)
{
    m_draw = true;
    setDraw(false);

    m_penWidth = 2;
    setCursor(Qt::OpenHandCursor);
    setZValue(20);

}

void GanttCurrentDtSlider::setScene(GanttScene *scene)
{
    if(!scene)
        return;

    m_scene = scene;
    scene->addItem(this);
    connect(m_scene,SIGNAL(limitsChanged(UtcDateTime,UtcDateTime)),this,SLOT(updateTextRect()));

    setSlidersRect(scene->sceneRect());


}

void GanttCurrentDtSlider::setDtLine(DtLine *dtline)
{
    _dtline = dtline;
}

QRectF GanttCurrentDtSlider::boundingRect() const
{
    return m_sliderShape.controlPointRect();
}

void GanttCurrentDtSlider::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(!m_draw)
        return;

    painter->setRenderHint(QPainter::Antialiasing,true);

    QBrush borderBrush(QColor(Qt::black).lighter(130));
    QBrush fillBrush(QColor(Qt::red));

    QPen pen(QBrush(QColor(Qt::black)), 1 ,Qt::SolidLine,Qt::SquareCap,Qt::MiterJoin);
    painter->setPen(pen);

    painter->fillPath(m_sliderShape,borderBrush);
    painter->fillPath(m_rhombus,fillBrush);
    painter->drawPath(m_rhombus);
    painter->save();
    painter->setBrush(QBrush(Qt::white));
    painter->drawRect(m_textRect);
    painter->setFont(m_scene->font());
    painter->drawText(m_textRect,m_dt.toString("HH:mm:ss dd.MM.yyyy"),QTextOption(Qt::AlignCenter));
    painter->restore();

}

QPainterPath GanttCurrentDtSlider::shape() const
{
    return m_sliderShape;
}

void GanttCurrentDtSlider::updateShape()
{
    QPainterPath path;
    qreal top = 0,
            bottom = m_slidersRect.height(),
            x = 0;

    path.moveTo(x - m_penWidth/2,top);
    path.lineTo(x + m_penWidth/2,top);
    path.lineTo(x + m_penWidth/2,bottom);
    path.lineTo(x - m_penWidth/2,bottom);
    path.lineTo(x - m_penWidth/2,top);

    qreal diagonal = (3.0/4)*DEFAULT_ITEM_HEIGHT;


    QPainterPath rhombus;
    rhombus.moveTo(QPointF(x - diagonal/2,DEFAULT_HEADER_HEIGHT - diagonal/2));
    rhombus.lineTo(QPointF(x ,DEFAULT_HEADER_HEIGHT - diagonal));
    rhombus.lineTo(QPointF(x + diagonal/2,DEFAULT_HEADER_HEIGHT - diagonal/2));
    rhombus.lineTo(QPointF(x ,DEFAULT_HEADER_HEIGHT));
    rhombus.lineTo(QPointF(x - diagonal/2,DEFAULT_HEADER_HEIGHT - diagonal/2));

    path.addPath(rhombus);

    prepareGeometryChange();
    m_rhombus = rhombus;

    updateTextRect();
    m_sliderShape.addRect(m_textRect);
    m_sliderShape = path;
}

void GanttCurrentDtSlider::updateTextRect()
{
    if(!m_scene)
        return;
    QFontMetrics fontMetrics(m_scene->font());
    QRect textRect;
    {
        textRect =  fontMetrics.tightBoundingRect(m_dt.toString("dd.MM.yyyy HH:mm:ss"));
        int adjust = 4;
        textRect.adjust(-adjust,-adjust,adjust,adjust);
        textRect.moveTo(10,1);
    }

    if(mapFromScene(m_scene->sceneRect().topRight()).x() < textRect.right())
    {
        textRect.moveTo(-textRect.width()-10,0);
    }
    m_textRect = textRect;
}




void GanttCurrentDtSlider::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!scene())
        return;

    if(cursor().shape() == Qt::ClosedHandCursor)
    {
        setDt(m_scene->posToDt(mapToScene(event->pos()).x()));
    }
}

void GanttCurrentDtSlider::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    setCursor(Qt::ClosedHandCursor);
}

void GanttCurrentDtSlider::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::OpenHandCursor);
    QGraphicsItem::mouseReleaseEvent(event);
}

void GanttCurrentDtSlider::setToBegin()
{
    setDt(m_minDt);
}

void GanttCurrentDtSlider::makeStep(int deltaVal)
{
    setDt(m_dt.addMicroseconds(deltaVal));
}

void GanttCurrentDtSlider::updateScenePos()
{
    if(outOfRange())
    {
        setVisible(false);
    }
    else
    {
        if(!m_scene)
        {
            Q_ASSERT(false);
            return;
        }

        setVisible(true);

        setPos((m_dt.isValid())?(m_scene->dtToPos(m_dt)):(m_slidersRect.left()), m_slidersRect.top());
    }
}

void GanttCurrentDtSlider::updateRange(const UtcDateTime &minDt, const UtcDateTime &maxDt)
{
    m_minDt = minDt;
    m_maxDt = maxDt;
}

void GanttCurrentDtSlider::updateRange(const GanttInfoNode *node)
{
    QPair<UtcDateTime,UtcDateTime> limits = GanttInfoItem::getLimits(node);
    updateRange(limits.first,limits.second);
    setDt(limits.first);
}

bool GanttCurrentDtSlider::outOfRange() const
{
    if(!m_scene)
    {
        Q_ASSERT(false);
        return true;
    }

    return !(m_dt.isValid()) || m_dt < _dtline->min() || m_dt > _dtline->max();
}

bool GanttCurrentDtSlider::outOfBounds(const UtcDateTime &dt) const
{
    return (dt<m_minDt || dt>m_maxDt);
}

void GanttCurrentDtSlider::setVisible(bool visible)
{
    if(visible == m_visible)
        return;

    m_visible = visible;
    if(m_draw)
        QGraphicsObject::setVisible(visible);
}

bool GanttCurrentDtSlider::draw() const
{
    return m_draw;
}

void GanttCurrentDtSlider::setDraw(bool draw)
{
    if(m_draw == draw)
        return;

    m_draw = draw;
    if(m_draw && m_visible)
        QGraphicsObject::setVisible(true);
    if(!m_draw)
        QGraphicsObject::setVisible(false);
    emit drawChanged(draw);

    update();

}


UtcDateTime GanttCurrentDtSlider::maxDt() const
{
    return m_maxDt;
}

void GanttCurrentDtSlider::moveToBegin()
{
    if(m_initialized)
        setDt(m_minDt);
}

void GanttCurrentDtSlider::moveToEnd()
{
    if(m_initialized)
        setDt(m_maxDt);
}

void GanttCurrentDtSlider::moveToRangeStart()
{
    if(!m_scene)
    {
        Q_ASSERT(false);
        return;
    }

    setDt(_dtline->min());
}

void GanttCurrentDtSlider::moveToRangeFinish()
{
    if(!m_scene)
    {
        Q_ASSERT(false);
        return;
    }

    setDt(_dtline->max());
}

//QPair<UtcDateTime,UtcDateTime> myMax(const QPair<UtcDateTime,UtcDateTime>&f,const QPair<UtcDateTime,UtcDateTime>&s)
//{
//    UtcDateTime resFirst,resSecond;

//    if(f.first.isValid() && s.first.isValid())
//        resFirst = qMin(f.first,s.first);
//    else if(f.first.isValid())
//        resFirst = f.first;
//    else
//        resFirst = s.first;

//    if(f.second.isValid() && s.second.isValid())
//        resSecond = qMax(f.second,s.second);
//    else if(f.second.isValid())
//        resSecond = f.second;
//    else
//        resSecond = s.second;

//    return QPair<UtcDateTime,UtcDateTime>(resFirst,resSecond);
//}

//QPair<UtcDateTime,UtcDateTime> GanttCurrentDtSlider::getLimits(const GanttInfoItem *root) const
//{
//    QPair<UtcDateTime,UtcDateTime> res;
//    if(!root)
//        return res;
//    const GanttInfoLeaf*leaf = qobject_cast<const GanttInfoLeaf*>(root);
//    if(leaf)
//    {
//        return qMakePair(leaf->start(),leaf->finish());
//    }

//    const GanttInfoNode *rNode = qobject_cast<const GanttInfoNode*>(root);
//    res = qMakePair(rNode->calcDt(),rNode->calcDt());

//    for(int i = 0; i<rNode->size(); ++i)
//    {
//        res = myMax(res,getLimits(rNode->child(i)));
//    }
//    return res;
//}

UtcDateTime GanttCurrentDtSlider::minDt() const
{
    return m_minDt;
}


bool GanttCurrentDtSlider::initialized() const
{
    return m_initialized;
}

qreal GanttCurrentDtSlider::relativePos() const
{
    if(m_dt.isValid() && m_minDt.isValid() && m_maxDt.isValid())
    {
        return (m_minDt.microsecondsTo(m_dt) * 1.0) / (m_minDt.microsecondsTo(m_maxDt));
    }
    return 0;
}

bool GanttCurrentDtSlider::setDt(UtcDateTime dt)
{
    if(dt < m_minDt)
        dt = m_minDt;
    if(dt > m_maxDt)
        dt = m_maxDt;

    m_dt = dt;

    if(!m_initialized)
        m_initialized = true;

    if(!m_scene)
    {
        Q_ASSERT(false);
        return false;
    }

    updateScenePos();

    emit dtChanged(dt);
    updateTextRect();
    return true;
}

UtcDateTime GanttCurrentDtSlider::dt() const
{
    return m_dt;
}

void GanttCurrentDtSlider::setSlidersRect(const QRectF &slidersRect)
{
    m_slidersRect = slidersRect;
    updateShape();
    updateScenePos();

}

void GanttCurrentDtSlider::setPos(const QPointF &pos)
{
    qreal x = pos.x();

    if(x < m_slidersRect.left())
        x = m_slidersRect.left();
    if(x > m_slidersRect.right())
        x = m_slidersRect.right();

    QGraphicsItem::setPos(x,pos.y());

    if(m_scene)
        m_scene->invalidate(QRectF(),QGraphicsScene::BackgroundLayer);
}

void GanttCurrentDtSlider::setPos(qreal x, qreal y)
{
    setPos(QPointF(x,y));
}
