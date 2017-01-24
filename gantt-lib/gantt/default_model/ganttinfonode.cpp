#include "ganttinfonode.h"
#include "ganttinfoitem.h"
#include "ganttinfoleaf.h"

#include "gantt-lib_global_values.h"

GanttInfoNode::GanttInfoNode(QObject *parent)
    :GanttInfoItem(parent)
{
    init();
}

GanttInfoNode::GanttInfoNode(const QString &title
                             , const UtcDateTime &start
                             , const TimeSpan &ts
                             , const QModelIndex &index
                             , const QColor &color
                             , GanttInfoNode *parentNode
                             , QObject *parent)
    : GanttInfoItem(title,start,ts,index,color,parentNode,parent)
{
    init();
}

GanttInfoLeaf *GanttInfoNode::leafAt(int index) const
{
    if(index>=size())
        return NULL;

    return dynamic_cast<GanttInfoLeaf*>(_items[index]);
}

GanttInfoNode *GanttInfoNode::nodeAt(int index) const
{
    if(index>=size())
        return NULL;

    return dynamic_cast<GanttInfoNode*>(_items[index]);
}

GanttInfoItem *GanttInfoNode::at(int index) const
{
    if(index>=size())
        return NULL;
    return _items[index];
}

GanttInfoItem *GanttInfoNode::operator[](int index) const
{
    return at(index);
}

int GanttInfoNode::size() const
{
    return _items.size();
}

bool GanttInfoNode::isEmpty() const
{
    return _items.isEmpty();
}

void GanttInfoNode::clear()
{
    _items.clear();
}


void GanttInfoNode::append(GanttInfoItem *item)
{
    if(!item)
        return;

    item->setParent(this);
    _items.append(item);
    emit itemsChanged();
}

void GanttInfoNode::append(const QList<GanttInfoItem *> &items)
{
    if(items.isEmpty())
        return;

    foreach(GanttInfoItem* item, items)
        item->setParent(this);
    _items.append(items);
    emit itemsChanged();
}

bool GanttInfoNode::removeOne(GanttInfoItem *item)
{
    if(_items.removeOne(item))
    {
        emit itemsChanged();
        return true;
    }
    return false;
}

int GanttInfoNode::columnCount() const
{
    return  1; //Only title  //GanttTreeModel::Fields_count;
}

GanttInfoNode *GanttInfoNode::node()
{
    return this;
}

qreal GanttInfoNode::height() const
{
    qreal nodeHeight = DEFAULT_ITEM_WIDTH;
    if(_expanded)
    {
        qreal childsHeight = 0;
        foreach(GanttInfoItem* item, _items)
        {
            childsHeight+=item->height();
        }
        return childsHeight + nodeHeight;
    }
    else
    {
        return nodeHeight;
    }
}

int GanttInfoNode::indexOf(const GanttInfoItem * p_item) const
{
    GanttInfoItem *p = const_cast<GanttInfoItem*>(p_item);
    return _items.indexOf(p);
}

void GanttInfoNode::onItemExpandingChange(int id)
{
    for(int i = id + 1; i < size(); ++i)
        at(i)->updatePos();
}

void GanttInfoNode::onSelfExpandingChange()
{
    for(int i = 0; i < size(); ++i)
        at(i)->updatePos();

    if(parent())
        parent()->onItemExpandingChange(row());
}

void GanttInfoNode::init()
{
    _expanded = false;

    connect(this,SIGNAL(itemsChanged()),this,SIGNAL(changed()));
    connect(this,SIGNAL(expanded()),this,SLOT(onSelfExpandingChange()));
    connect(this,SIGNAL(collapsed()),this,SLOT(onSelfExpandingChange()));
}

void GanttInfoNode::callForEachItemRecursively(void (*func)(GanttInfoItem *))
{
    if(!func)
        return;

    foreach(GanttInfoItem* item, _items)
        item->callForEachItemRecursively(func);

    (*func)(this);
}

void GanttInfoNode::updatePos()
{
    GanttInfoItem::updatePos();
    foreach(GanttInfoItem* item, _items)
        item->updatePos();
}

void GanttInfoNode::setExpanded(bool newExpanded)
{
    if(!_expanded && newExpanded){
        _expanded = newExpanded;
        emit expanded();
    }
    else if(_expanded && !newExpanded){
        _expanded = newExpanded;
        emit collapsed();
    }
    else
        _expanded = newExpanded;
}

void GanttInfoNode::changeExpanding()
{
    setExpanded(!isExpanded());
}

bool GanttInfoNode::isExpanded() const
{
    return _expanded;
}


