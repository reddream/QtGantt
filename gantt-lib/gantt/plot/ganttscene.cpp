#include "ganttscene.h"
#include "dtline.h"
#include "ganttinfotree.h"

#include "gantt-lib_global_values.h"

#include "ganttintervalgraphicsobject.h"
#include "ganttcalcgraphicsobject.h"
#include "ganttdtcrossobject.h"
#include "gantthovergraphicsobject.h"

#include "ganttinfoleaf.h"
#include "ganttinfonode.h"


#include <QGraphicsView>

#include <QScrollBar>
#include <QSize>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QtCore/qmath.h>
#include <QDebug>


void GanttScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawBackground(painter,rect);

    qreal   bgBottom = rect.bottom()
            ,bgLeft = rect.left() - 1
            ,bgRight = rect.right() + 1;

    QPen pen(Qt::gray, qreal(0.5));
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);

    qreal startY = (qCeil(rect.y() / DEFAULT_ITEM_HEIGHT))*DEFAULT_ITEM_HEIGHT;

    while(startY<bgBottom)
    {
        painter->drawLine(QPointF(bgLeft,startY),QPointF(bgRight,startY));
        startY+=DEFAULT_ITEM_HEIGHT;
    }
}


GanttGraphicsObject *GanttScene::itemByInfo(const GanttInfoItem *key) const
{
    return m_itemByInfo.value(key);
}


void GanttScene::updateSceneRect()
{
    if(!sceneHaveItems())
    {
        setSceneRect(0,0,sceneRect().width(),0);
    }
    else
    {
        QRectF elemsRect = elementsBoundingRect();
        setSceneRect(0,0,sceneRect().width(),elemsRect.height());
    }

    updateSliderRect();
}

void GanttScene::updateSceneItems()
{
    updateItems();
    updateSliderRect();
    update();
}

void GanttScene::makeStep(int step)
{
    if(m_slider)
        m_slider->makeStep(step);
}

void GanttScene::moveSliderToNextEventStart()
{
    if(m_slider)
    {
        const GanttInfoLeaf* nextEventInfo = nextEvent(slidersDt());
        if(nextEventInfo)
        {
            m_slider->setDt(nextEventInfo->start());
        }
        else
            m_slider->moveToEnd();
    }
}

void GanttScene::moveSliderToPrevEventFinish()
{
    if(m_slider)
    {
        const GanttInfoLeaf* prevEventInfo = prevEvent(slidersDt());
        if(prevEventInfo)
        {
            m_slider->setDt(prevEventInfo->finish());
        }
        else
            m_slider->moveToBegin();
    }
}

void GanttScene::moveSliderToViewStart()
{
    if(m_slider)
        m_slider->moveToRangeStart();
}

void GanttScene::moveSliderToViewFinish()
{
    if(m_slider)
        m_slider->moveToRangeFinish();
}

void GanttScene::moveSliderToStart()
{
    if(m_slider)
        m_slider->moveToBegin();
}

void GanttScene::setCurrentByInfo(GanttInfoItem *info)
{
    setCurrentItem(itemByInfo(info));
}

UtcDateTime GanttScene::slidersDt() const
{
    return m_slider->dt();
}


void GanttScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    if(event->buttons() & Qt::LeftButton)
    {
        QGraphicsObject *object = objectForPos(event->scenePos());
        if(object)
        {
            setCurrentItem(object);
        }
    }

    mouseMoveEvent(event);
}

void GanttScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseDoubleClickEvent(event);
    if(event->buttons() & Qt::LeftButton)
    {
        GanttGraphicsObject *object = objectForPos(event->scenePos());
        if(object)
        {
            GanttInfoNode *node = object->info()->node();
            if(node->parent()) // not root
                node->changeExpanding();
        }
    }

    mouseMoveEvent(event);
}

void GanttScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() & Qt::RightButton)
    {
        if(_view)
        {
            QRectF viewRect = _view->mapToScene(_view->viewport()->geometry()).boundingRect();
            if(viewRect.contains(event->scenePos()) && m_crossObject)
            {
                m_crossObject->setVisible(true);
                m_crossObject->setPos(event->scenePos());
            }
            else
            {
                m_crossObject->setVisible(false);
            }
        }
        else
        {
            qCritical("m_view is null");
        }
    }


    QGraphicsScene::mouseMoveEvent(event);
}

void GanttScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        if(m_crossObject)
        {
            m_crossObject->setVisible(false);
        }
    }

    QGraphicsScene::mouseReleaseEvent(event);
}

void GanttScene::connectDtLine()
{
    connect(_dtline,SIGNAL(changed()),this,SLOT(updateSceneItems()));
    connect(_dtline,SIGNAL(minChanged()),this,SLOT(updateSceneItems()));
    connect(_dtline,SIGNAL(timeSpanChanged()),this,SLOT(updateSceneItems()));

}

void GanttScene::onVisItemDestroyed()
{
    updateSceneRect();
}

void GanttScene::onLeafStartChanged(/*const UtcDateTime& lastStart*/)
{
    GanttInfoLeaf* p_leaf = qobject_cast<GanttInfoLeaf*>(sender());

    if(!p_leaf)
        return;

    m_infoByStart.remove(m_infoByStart.key(p_leaf));
    m_infoByStart.insert(p_leaf->start(),p_leaf);

}

void GanttScene::onLeafFinishChanged(/*const UtcDateTime& lastFinish*/)
{
    GanttInfoLeaf* p_leaf = qobject_cast<GanttInfoLeaf*>(sender());

    if(!p_leaf)
        return;

    m_infoByFinish.remove(m_infoByFinish.key(p_leaf));
    m_infoByFinish.insert(p_leaf->start(),p_leaf);
}

void GanttScene::setTreeInfo(GanttInfoTree *treeInfo)
{
    _treeInfo = treeInfo;
}


QGraphicsItem *GanttScene::currentItem() const
{
    return m_currentItem;
}

bool GanttScene::isVisible(const QGraphicsItem *which) const
{
    if(!_view)
        return false;

    QRectF viewRect = _view->mapToScene(_view->viewport()->geometry()).boundingRect()
            .adjusted(0,DEFAULT_HEADER_HEIGHT,0,0);

    return viewRect.contains(which->sceneBoundingRect());
}

QRectF GanttScene::elementsBoundingRect()
{
    QRectF res;
    foreach(GanttIntervalGraphicsObject* object,m_items)
        res|=object->sceneBoundingRect();
    foreach(GanttCalcGraphicsObject *object,m_calcItems)
        res|=object->sceneBoundingRect();
    return res;
}

void GanttScene::clear()
{
    m_currentItem = NULL;
}


bool GanttScene::sceneHaveItems() const
{
    return !(m_items.isEmpty() && m_calcItems.isEmpty());
}

void GanttScene::onViewResized(const QSize &newSize)
{
    HFitScene::onViewResized(newSize);
    updateSceneItems();
}

void GanttScene::setCurrentItem(QGraphicsObject *currentItem)
{
    if(m_currentItem == currentItem)
        return;

    QGraphicsItem *lastItem = m_currentItem;
    GanttInfoItem *info = NULL;
    m_currentItem = currentItem;
    if(lastItem)
    {
        lastItem->update();
    }

    if(m_currentItem)
    {
        if(GanttIntervalGraphicsObject *graphicsObject = dynamic_cast<GanttIntervalGraphicsObject*>(m_currentItem.data()))
            info = graphicsObject->info();
        if(GanttCalcGraphicsObject *graphicsObject = dynamic_cast<GanttCalcGraphicsObject*>(m_currentItem.data()))
            info = graphicsObject->info();

        QRectF itemRect = m_currentItem->mapToScene(m_currentItem->boundingRect()).boundingRect();
        if(_view && !isVisible(m_currentItem))
        {
            _view->ensureVisible(itemRect,0,0);
        }
        m_currentItem->update();
    }

    m_hoverObject->setItem(info);   // if no currentItme sets to NULL

    if(lastItem != currentItem)
        emit currentItemChanged(info);
}

GanttGraphicsObject *GanttScene::objectForPos(const QPointF &pos)
{
    GanttGraphicsObject *object;
    if((object = dynamic_cast<GanttGraphicsObject*>(itemAt(pos))))
        return object;
    return itemByInfo(_treeInfo->infoForVPos(pos.y()));
}
const QList<GanttIntervalGraphicsObject *>& GanttScene::dtItems() const
{
    return m_items;
}

void GanttScene::setDrawCurrentDtSlider(bool enable)
{
    if(m_slider)
        m_slider->setDraw(enable);
}


void GanttScene::updateSlider()
{
    if(!m_slider)
        return;

    m_slider->updateScenePos();
}

void GanttScene::updateItems()
{
    foreach(GanttIntervalGraphicsObject *o, m_items)
        o->updateItemGeometry();

    foreach(GanttCalcGraphicsObject *o, m_calcItems)
        o->updateItemGeometry();
}

void GanttScene::onGraphicsItemPress()
{
    GanttIntervalGraphicsObject *item = qobject_cast<GanttIntervalGraphicsObject*>(sender());
    if(item)
    {
        setCurrentItem(item);
    }

    GanttCalcGraphicsObject *calcItem = qobject_cast<GanttCalcGraphicsObject*>(sender());
    if(calcItem)
    {
        setCurrentItem(calcItem);
    }
}

void GanttScene::onGraphicsItemHoverEnter()
{
    GanttIntervalGraphicsObject *item = qobject_cast<GanttIntervalGraphicsObject*>(sender());
    if(item)
    {
//        setCurrentItem(item);
        emit graphicsItemHoverEnter(item->info());
    }

    GanttCalcGraphicsObject *calcItem = qobject_cast<GanttCalcGraphicsObject*>(sender());
    if(calcItem)
    {
//        setCurrentItem(calcItem);
        emit graphicsItemHoverEnter(calcItem->info());
    }


}

void GanttScene::onGraphicsItemHoverLeave()
{
    GanttIntervalGraphicsObject *item = qobject_cast<GanttIntervalGraphicsObject*>(sender());
    if(item)
        emit graphicsItemHoverLeave(item->info());

    GanttCalcGraphicsObject *calcItem = qobject_cast<GanttCalcGraphicsObject*>(sender());
    if(calcItem)
        emit graphicsItemHoverLeave(calcItem->info());
}

void GanttScene::onInfoDelete()
{
    GanttInfoItem* item = static_cast<GanttInfoItem*>(sender());
    onItemRemoved(item);
}



void GanttScene::onInfoLeafDelete()
{
    const GanttInfoLeaf* leaf = static_cast<const GanttInfoLeaf*>(sender());
    removeByInfoLeaf(leaf);
}


const GanttInfoLeaf *GanttScene::nextEvent(const UtcDateTime &curDt) const
{
    if(curDt.isValid())
    {
        QMap<UtcDateTime,const GanttInfoLeaf*>::const_iterator it = m_infoByStart.constBegin();
        while (it != m_infoByStart.constEnd())
        {
            if(curDt < it.key())
                return it.value();
            ++it;
        }
    }

    return NULL;
}

const GanttInfoLeaf *GanttScene::prevEvent(const UtcDateTime &curDt) const
{
    if(curDt.isValid())
    {
        QMapIterator<UtcDateTime,const GanttInfoLeaf*> it(m_infoByFinish);
        it.toBack();
        while (it.hasPrevious()) {
            it.previous();
            if(curDt > it.key())
                return it.value();
        }
    }

    return NULL;
}

void GanttScene::onItemRemoved(GanttInfoItem *item)
{
    if(!item)
        return;
    const GanttInfoLeaf *leaf = qobject_cast<const GanttInfoLeaf*>(item);
    if(leaf)
    {
        GanttIntervalGraphicsObject* graphicsItem = qobject_cast<GanttIntervalGraphicsObject*>(itemByInfo(leaf));
        if(graphicsItem)
            m_items.removeOne(graphicsItem);
        m_itemByInfo.remove(leaf);
        m_infoByStart.remove(m_infoByStart.key(leaf));
        m_infoByFinish.remove(m_infoByFinish.key(leaf));
        if(graphicsItem)
            graphicsItem->deleteLater();
    }

    const GanttInfoNode *node = qobject_cast<const GanttInfoNode *>(item);
    if(node)
    {
        GanttCalcGraphicsObject* graphicsItem = qobject_cast<GanttCalcGraphicsObject*>(itemByInfo(node));
        if(graphicsItem)
            m_calcItems.removeOne(graphicsItem);
        m_itemByInfo.remove(node);
        if(graphicsItem)
            graphicsItem->deleteLater();
    }
}

void GanttScene::onEndInsertItems()
{
//    m_slider->updateRange();   /// TODO
    m_slider->setToBegin();
    updateSceneRect();
}

void GanttScene::onEndRemoveItems()
{
    updateSceneRect();
}

void GanttScene::removeByInfoLeaf(const GanttInfoLeaf *leaf)
{
    if(!leaf)
        return;

    GanttIntervalGraphicsObject* graphicsItem = qobject_cast<GanttIntervalGraphicsObject*>(itemByInfo(leaf));
    if(graphicsItem)
        m_items.removeOne(graphicsItem);
    m_itemByInfo.remove(leaf);
    m_infoByStart.remove(m_infoByStart.key(leaf));
    m_infoByFinish.remove(m_infoByFinish.key(leaf));
    if(graphicsItem)
        graphicsItem->deleteLater();
}

void GanttScene::updateSliderRect()
{
    if(!m_slider || views().isEmpty())
        return;

    QGraphicsView* p_view = views()[0];

    m_slider->setSlidersRect(QRectF( MIN_WIDTH_FOR_TIME_VISUALIZING/2
                             ,p_view->verticalScrollBar()->value()
                             ,width()-MIN_WIDTH_FOR_TIME_VISUALIZING
                             ,p_view->height() ) );
    update();
}



void GanttScene::init()
{
    connectDtLine();
    m_currentItem = NULL;

    setSceneRect(0,0,GANTTSCENE_MIN_WIDTH,0);

    m_slider = new GanttCurrentDtSlider(this,_dtline);
    m_crossObject = new GanttDtCrossObject(this);
    m_hoverObject = new GanttHoverGraphicsObject(this);

    connect(m_slider,SIGNAL(dtChanged(UtcDateTime)),this,SIGNAL(currentDtChanged(UtcDateTime)));

    updateSliderRect();
}

GanttScene::GanttScene(GanttGraphicsView *view, DtLine *dtline, QObject *parent)
    : HFitScene(view, parent),_dtline(dtline)
{
    init();
}

int GanttScene::dtToPos(const UtcDateTime &dt) const
{
    return _dtline->dtToPos(dt);
}

UtcDateTime GanttScene::posToDt(int pos) const
{
    return _dtline->posToDt(pos);
}

void GanttScene::onItemAdded(GanttInfoItem *item)
{
    if(!item)
        return;

    connect(item,SIGNAL(destroyed(QObject*)),this,SLOT(onVisItemDestroyed()));
    connect(item,SIGNAL(expanded()),this,SLOT(updateSceneRect()));
    connect(item,SIGNAL(collapsed()),this,SLOT(updateSceneRect()));

    GanttInfoLeaf *leaf = dynamic_cast<GanttInfoLeaf*>(item);
    GanttGraphicsObject *p_item = NULL;
    if(leaf)
    {
        GanttIntervalGraphicsObject *p;
        p_item = p = new GanttIntervalGraphicsObject(leaf);

        m_items.append(p);
        m_itemByInfo.insert(leaf,p);
        m_infoByStart.insert(leaf->start(),leaf);
        m_infoByFinish.insert(leaf->finish(),leaf);
    }
    else
    {
        GanttInfoNode *node = dynamic_cast<GanttInfoNode*>(item);
        if(node)
        {
            if(node->hasStart())
            {
                GanttCalcGraphicsObject *p;
                p_item = p = new GanttCalcGraphicsObject(node);

                m_calcItems.append(p);
                m_itemByInfo.insert(node,p);
            }
        }
    }
    if(p_item){
        p_item->setDtLine(_dtline);
        p_item->setScene(this);

        connect(p_item,SIGNAL(graphicsItemHoverEnter()),this,SLOT(onGraphicsItemHoverEnter()));
        connect(p_item,SIGNAL(graphicsItemHoverLeave()),this,SLOT(onGraphicsItemHoverLeave()));
        connect(p_item,SIGNAL(graphicsItemPressed()),this,SLOT(onGraphicsItemPress()));

        p_item->updateItemGeometry();
    }
}

