#include "ganttinfonode.h"
#include "gantttreemodel.h"

#include "gantt-lib_global_values.h"

GanttInfoNode::GanttInfoNode(QObject *parent)
    :GanttInfoItem(parent)
{
    init();
}

GanttInfoNode::GanttInfoNode(const QString &title, const UtcDateTime &start, const UtcDateTime &finish
                             , const QModelIndex &index
                             , const QColor &color
                             , GanttInfoNode *parentNode
                             , QObject *parent)
    : GanttInfoItem(title,start,finish,index,color,parentNode,parent)
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

void GanttInfoNode::init()
{
    _expanded = false;

    connect(this,SIGNAL(itemsChanged()),this,SIGNAL(changed()));
}

void GanttInfoNode::callForEachItemRecursively(void (*func)(GanttInfoItem *))
{
    if(!func)
        return;

    foreach(GanttInfoItem* item, _items)
        item->callForEachItemRecursively(func);

    (*func)(this);
}
void GanttInfoNode::setExpanded(bool newExpanded)
{
    if(!_expanded && newExpanded)
        emit expanded();
    if(_expanded && !newExpanded)
        emit collapsed();

    _expanded = newExpanded;
}

bool GanttInfoNode::expanded() const
{
    return _expanded;
}


