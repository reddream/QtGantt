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

#include <QApplication>
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
    return _itemByInfo.value(key);
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
}

void GanttScene::updateSceneItems()
{
    updateSlider();
    _dtline->updateCurrentDtPath();
    updateItems();
    update();
}

void GanttScene::makeStep(int step)
{
    if(_playerCurrent)
        _playerCurrent->makeStep(step);
}

void GanttScene::moveSliderToNextEventStart()
{
    if(_playerCurrent)
    {
        const GanttInfoLeaf* nextEventInfo = nextEvent(slidersDt());
        if(nextEventInfo)
        {
            _playerCurrent->setDt(nextEventInfo->start());
        }
        else
            _playerCurrent->moveToEnd();
    }
}

void GanttScene::moveSliderToPrevEventFinish()
{
    if(_playerCurrent)
    {
        const GanttInfoLeaf* prevEventInfo = prevEvent(slidersDt());
        if(prevEventInfo)
        {
            _playerCurrent->setDt(prevEventInfo->finish());
        }
        else
            _playerCurrent->moveToBegin();
    }
}

void GanttScene::moveSliderToViewStart()
{
    if(_playerCurrent)
        _playerCurrent->moveToRangeStart();
}

void GanttScene::moveSliderToViewFinish()
{
    if(_playerCurrent)
        _playerCurrent->moveToRangeFinish();
}

void GanttScene::moveSliderToStart()
{
    if(_playerCurrent)
        _playerCurrent->moveToBegin();
}

void GanttScene::setCurrentByInfo(GanttInfoItem *info)
{
    setCurrentItem(itemByInfo(info));
}

UtcDateTime GanttScene::slidersDt() const
{
    return _playerCurrent->dt();
}


void GanttScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    if(event->button() == Qt::LeftButton){
        QGraphicsObject *object = objectForPos(event->scenePos());
        if(object)
            setCurrentItem(object);
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
    if(event->buttons() & Qt::RightButton){
        if(_view)
        {
            QRectF viewRect = _view->mapToScene(_view->viewport()->geometry()).boundingRect();
            if(viewRect.contains(event->scenePos()))
            {
                _crossObject->setVisible(true);
                _crossObject->setPos(event->scenePos());
            }
            else
            {
                _crossObject->setVisible(false);
            }
        }
        else
        {
            qCritical("m_view is null");
        }
    }
    if(event->buttons() & Qt::MiddleButton){
        _dtline->slide((event->lastScenePos().x() - event->scenePos().x()) * 1. / width());
    }

    QGraphicsScene::mouseMoveEvent(event);
}

void GanttScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton){
        _crossObject->setVisible(false);
    }

    QGraphicsScene::mouseReleaseEvent(event);
}

void GanttScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if(QApplication::keyboardModifiers() & Qt::ControlModifier){
        _dtline->zoom(event->delta(), event->scenePos().x() * 1. / width());
        return; // forbid VSrollBar
    }


    QGraphicsScene::wheelEvent(event);
}

void GanttScene::onTreeInfoReset()
{
    clear();
    addInfoItem(_treeInfo->root());
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

    _infoByStart.remove(_infoByStart.key(p_leaf));
    _infoByStart.insert(p_leaf->start(),p_leaf);

}

void GanttScene::onLeafFinishChanged(/*const UtcDateTime& lastFinish*/)
{
    GanttInfoLeaf* p_leaf = qobject_cast<GanttInfoLeaf*>(sender());

    if(!p_leaf)
        return;

    _infoByFinish.remove(_infoByFinish.key(p_leaf));
    _infoByFinish.insert(p_leaf->start(),p_leaf);
}

void GanttScene::setTreeInfo(GanttInfoTree *treeInfo)
{
    _treeInfo = treeInfo;
}


QGraphicsItem *GanttScene::currentItem() const
{
    return _currentItem;
}

bool GanttScene::isVisible(const QGraphicsItem *which) const
{
    if(!_view)
        return false;

    QRectF viewRect = _view->mapToScene(_view->viewport()->geometry()).boundingRect();

    return viewRect.contains(which->sceneBoundingRect());
}

QRectF GanttScene::elementsBoundingRect()
{
    QRectF res;
    foreach(GanttIntervalGraphicsObject* object,_items)
        res|=object->sceneBoundingRect();
    foreach(GanttCalcGraphicsObject *object,_calcItems)
        res|=object->sceneBoundingRect();
    return res;
}

void GanttScene::clear()
{
    _currentItem = NULL;

    removePersistentItems();
    QGraphicsScene::clear();
    _items.clear();
    _calcItems.clear();
    _itemByInfo.clear();
    _infoByFinish.clear();
    _infoByStart.clear();
    addPersistentItems();
}


bool GanttScene::sceneHaveItems() const
{
    return !(_items.isEmpty() && _calcItems.isEmpty());
}

void GanttScene::onViewResized(const QSize &newSize)
{
    HFitScene::onViewResized(newSize);
    updateSceneItems();
    updateSliderHeight();
}

void GanttScene::setCurrentItem(QGraphicsObject *currentItem)
{
    if(_currentItem == currentItem)
        return;

    QGraphicsItem *lastItem = _currentItem;
    GanttInfoItem *info = NULL;
    _currentItem = currentItem;
    if(lastItem)
    {
        lastItem->update();
    }

    if(_currentItem)
    {
        if(GanttIntervalGraphicsObject *graphicsObject =
                dynamic_cast<GanttIntervalGraphicsObject*>(_currentItem.data()))
            info = graphicsObject->info();
        if(GanttCalcGraphicsObject *graphicsObject =
                dynamic_cast<GanttCalcGraphicsObject*>(_currentItem.data()))
            info = graphicsObject->info();

        QRectF itemRect = _currentItem->mapToScene(_currentItem->boundingRect()).boundingRect();
        if(_view && !isVisible(_currentItem))
        {
            _view->ensureVisible(itemRect,0,0);
        }
        _currentItem->update();
    }

    _hoverObject->setItem(info);   // if no currentItme sets to NULL

    if(lastItem != currentItem)
        emit currentItemChanged(info);
}

GanttGraphicsObject *GanttScene::objectForPos(const QPointF &pos)
{
    GanttGraphicsObject *object;
    if((object = dynamic_cast<GanttGraphicsObject*>(itemAt(pos)))){
        return object;
    }
    return itemByInfo(_treeInfo->infoForVPos(pos.y()));
}
const QList<GanttIntervalGraphicsObject *>& GanttScene::dtItems() const
{
    return _items;
}

void GanttScene::setDrawCurrentDtSlider(bool enable)
{
    if(_playerCurrent)
        _playerCurrent->setDraw(enable);
}


void GanttScene::updateSlider()
{
    if(!_playerCurrent)
        return;

    _playerCurrent->updateScenePos();
}

void GanttScene::updateItems()
{
    foreach(GanttIntervalGraphicsObject *o, _items)
        o->updateItemGeometry();

    foreach(GanttCalcGraphicsObject *o, _calcItems)
        o->updateItemGeometry();
}

void GanttScene::createPersistentItems()
{
    _playerCurrent = new GanttCurrentDtSlider(this,_dtline);
    _crossObject = new GanttDtCrossObject(this);
    _hoverObject = new GanttHoverGraphicsObject(this);

    connect(_playerCurrent,SIGNAL(dtChanged(UtcDateTime)),this,SIGNAL(currentDtChanged(UtcDateTime)));
    connect(_playerCurrent, SIGNAL(dtChanged(UtcDateTime)),_dtline,SLOT(setCurrentDt(UtcDateTime)));
    connect(_dtline,SIGNAL(dtChangedManually(UtcDateTime)),_playerCurrent,SLOT(setDt(UtcDateTime)));
    connect(_playerCurrent,SIGNAL(drawChanged(bool)),_dtline,SLOT(showCurrentDt(bool)));
}

void GanttScene::addPersistentItems()
{
    addItem(_playerCurrent);
    addItem(_crossObject);
    addItem(_hoverObject);
}

void GanttScene::removePersistentItems()
{
    removeItem(_playerCurrent);
    removeItem(_crossObject);
    removeItem(_hoverObject);
}

void GanttScene::addInfoItem(GanttInfoItem *item)
{
    onItemAdded(item);
    GanttInfoNode *node = qobject_cast<GanttInfoNode*>(item);
    if(node){
        for(int i = 0; i < node->size(); ++i){
            addInfoItem(node->at(i));
        }
    }

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
        QMap<UtcDateTime,const GanttInfoLeaf*>::const_iterator it = _infoByStart.constBegin();
        while (it != _infoByStart.constEnd())
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
        QMapIterator<UtcDateTime,const GanttInfoLeaf*> it(_infoByFinish);
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
            _items.removeOne(graphicsItem);
        _itemByInfo.remove(leaf);
        _infoByStart.remove(_infoByStart.key(leaf));
        _infoByFinish.remove(_infoByFinish.key(leaf));
        if(graphicsItem)
            graphicsItem->deleteLater();
    }

    const GanttInfoNode *node = qobject_cast<const GanttInfoNode *>(item);
    if(node)
    {
        GanttCalcGraphicsObject* graphicsItem = qobject_cast<GanttCalcGraphicsObject*>(itemByInfo(node));
        if(graphicsItem)
            _calcItems.removeOne(graphicsItem);
        _itemByInfo.remove(node);
        if(graphicsItem)
            graphicsItem->deleteLater();
    }
}

void GanttScene::onEndInsertItems()
{
    QPair<UtcDateTime,UtcDateTime> limits = _treeInfo->root()->getLimits();
    _playerCurrent->updateRange(limits.first, limits.second - limits.first);
    _playerCurrent->setToBegin();
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
        _items.removeOne(graphicsItem);
    _itemByInfo.remove(leaf);
    _infoByStart.remove(_infoByStart.key(leaf));
    _infoByFinish.remove(_infoByFinish.key(leaf));
    if(graphicsItem)
        graphicsItem->deleteLater();
}


void GanttScene::updateSliderHeight()
{
    if(!_playerCurrent || views().isEmpty())
        return;

    _playerCurrent->setHeight(views()[0]->height());
    update();
}



void GanttScene::init()
{
    connectDtLine();
    _currentItem = NULL;

    setSceneRect(0,0,GANTTSCENE_MIN_WIDTH,0);

    createPersistentItems();
}

GanttScene::GanttScene(GanttGraphicsView *view, GanttDtLine *dtline, QObject *parent)
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

        _items.append(p);
        _itemByInfo.insert(leaf,p);
        _infoByStart.insert(leaf->start(),leaf);
        _infoByFinish.insert(leaf->finish(),leaf);
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

                _calcItems.append(p);
                _itemByInfo.insert(node,p);
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

