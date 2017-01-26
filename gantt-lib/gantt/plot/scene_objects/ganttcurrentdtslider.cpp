#include "ganttcurrentdtslider.h"
#include "gantt-lib_global_values.h"
#include "ganttscene.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QPair>

#include <QDebug>

void GanttCurrentDtSlider::init()
{
    _scene = NULL;
    _dtline = NULL;
    _height = 0;
    setDraw(false);

    setCursor(Qt::PointingHandCursor);
    setZValue(20);

    initPath();
}

GanttCurrentDtSlider::GanttCurrentDtSlider(QGraphicsItem* parent) :
    QGraphicsObject(parent)
{
    init();
}

GanttCurrentDtSlider::GanttCurrentDtSlider(GanttScene *scene, DtLine *dtline, QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    init();
    setDtLine(dtline);
    setScene(scene);
}

void GanttCurrentDtSlider::setScene(GanttScene *scene)
{
    if(!scene)
        return;

    _scene = scene;
    scene->addItem(this);

    if(!scene->views().isEmpty())
        setHeight(scene->views()[0]->height());
}

void GanttCurrentDtSlider::setDtLine(DtLine *dtline)
{
    if(!dtline){
        qWarning("GanttCurrentDtSlider::setDtLine NULL");
        return;
    }

    _dtline = dtline;

    connect(_dtline,SIGNAL(rangeChanged()),this,SLOT(updateTextRect()));
}

QRectF GanttCurrentDtSlider::boundingRect() const
{
    return QRectF(0,0,5,_height);
}

void GanttCurrentDtSlider::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(!_draw || !_scene)
        return;

    painter->save();
    QPen pen(QBrush(QColor(Qt::red)), 1 ,Qt::DashLine,Qt::SquareCap,Qt::MiterJoin);
    painter->setPen(pen);
    painter->drawLine(0,0,0, _scene->view()->rect().bottom());
    painter->restore();

    return;//drawing textrect
    painter->save();
    painter->setBrush(QBrush(Qt::white));
    painter->drawRect(m_textRect);
    painter->setFont(_scene->font());
    painter->drawText(m_textRect,m_dt.toString("HH:mm:ss dd.MM.yyyy"),QTextOption(Qt::AlignCenter));
    painter->restore();

}

void GanttCurrentDtSlider::updateTextRect()
{
    if(!_scene)
        return;
    QFontMetrics fontMetrics(_scene->font());
    QRect textRect;
    {
        textRect =  fontMetrics.tightBoundingRect(m_dt.toString("dd.MM.yyyy HH:mm:ss"));
        int adjust = 4;
        textRect.adjust(-adjust,-adjust,adjust,adjust);
        textRect.moveTo(10,1);
    }

    if(mapFromScene(_scene->sceneRect().topRight()).x() < textRect.right())
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
        setDt(_scene->posToDt(mapToScene(event->pos()).x()));
    }
}

void GanttCurrentDtSlider::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    setCursor(Qt::ClosedHandCursor);
}

void GanttCurrentDtSlider::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::PointingHandCursor);
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
        setVisible(false);
    else
    {
        if(!_scene){
            return;
        }
        setVisible(true);
        setPos((m_dt.isValid())?(_scene->dtToPos(m_dt)):0, 0);
    }
}

void GanttCurrentDtSlider::updateRange(const UtcDateTime &minDt, const TimeSpan &ts)
{
    m_minDt = minDt;
    m_maxDt = minDt + ts;
}


bool GanttCurrentDtSlider::outOfRange() const
{
    if(!_scene)
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

void GanttCurrentDtSlider::initPath()
{

}

int GanttCurrentDtSlider::height() const
{
    return _height;
}

void GanttCurrentDtSlider::setHeight(int height)
{
    prepareGeometryChange();
    _height = height;
}

void GanttCurrentDtSlider::setVisible(bool visible)
{
    if(visible == _visible)
        return;

    _visible = visible;
    if(_draw)
        QGraphicsObject::setVisible(visible);
}

bool GanttCurrentDtSlider::draw() const
{
    return _draw;
}

void GanttCurrentDtSlider::setDraw(bool draw)
{
    _draw = draw;
    if(_draw && _visible)
        QGraphicsObject::setVisible(true);
    if(!_draw)
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
    if(_initialized)
        setDt(m_minDt);
}

void GanttCurrentDtSlider::moveToEnd()
{
    if(_initialized)
        setDt(m_maxDt);
}

void GanttCurrentDtSlider::moveToRangeStart()
{
    if(!_scene)
    {
        Q_ASSERT(false);
        return;
    }

    setDt(_dtline->min());
}

void GanttCurrentDtSlider::moveToRangeFinish()
{
    if(!_scene)
    {
        Q_ASSERT(false);
        return;
    }

    setDt(_dtline->max());
}

UtcDateTime GanttCurrentDtSlider::minDt() const
{
    return m_minDt;
}


bool GanttCurrentDtSlider::initialized() const
{
    return _initialized;
}

qreal GanttCurrentDtSlider::relativePos() const
{
    if(m_dt.isValid() && m_minDt.isValid() && m_maxDt.isValid())
        return (m_minDt.microsecondsTo(m_dt) * 1.0) / (m_minDt.microsecondsTo(m_maxDt));
    return 0;
}

bool GanttCurrentDtSlider::setDt(UtcDateTime dt)
{
    if(dt < m_minDt)
        dt = m_minDt;
    if(dt > m_maxDt)
        dt = m_maxDt;

    m_dt = dt;

    if(!_initialized)
        _initialized = true;

    if(!_scene)
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

void GanttCurrentDtSlider::setPos(const QPointF &pos)
{
    QGraphicsItem::setPos(pos.x(),pos.y());

    if(_scene)
        _scene->invalidate(QRectF(),QGraphicsScene::BackgroundLayer);
}

void GanttCurrentDtSlider::setPos(qreal x, qreal y)
{
    setPos(QPointF(x,y));
}
