#include "ganttinfonode.h"
#include "gantttreemodel.h"

#include "ganttglobalvalues.h"

GanttInfoNode::GanttInfoNode(QObject *parent)
    :GanttInfoItem(parent)
{
    m_isExpanded = false;

    connect(this,SIGNAL(itemsChanged()),this,SIGNAL(changed()));
    connect(this,SIGNAL(calcDtChanged()),this,SIGNAL(changed()));
}

GanttInfoLeaf *GanttInfoNode::leafAt(int index) const
{
    if(index>=size())
        return NULL;

    return dynamic_cast<GanttInfoLeaf*>(m_items[index]);
}

GanttInfoNode *GanttInfoNode::nodeAt(int index) const
{
    if(index>=size())
        return NULL;

    return dynamic_cast<GanttInfoNode*>(m_items[index]);
}

GanttInfoItem *GanttInfoNode::child(int index) const
{
    if(index>=size())
        return NULL;
    return m_items[index];
}

int GanttInfoNode::size() const
{
    return m_items.size();
}

bool GanttInfoNode::isEmpty() const
{
    return m_items.isEmpty();
}

void GanttInfoNode::clear()
{
    m_items.clear();
}


void GanttInfoNode::append(GanttInfoItem *item)
{
    if(!item)
        return;

    item->setParent(this);
    m_items.append(item);
    emit itemsChanged();
}

void GanttInfoNode::append(const QList<GanttInfoItem *> &items)
{
    if(items.isEmpty())
        return;

    foreach(GanttInfoItem* item, items)
        item->setParent(this);
    m_items.append(items);
    emit itemsChanged();
}

bool GanttInfoNode::removeOne(GanttInfoItem *item)
{
    if(m_items.removeOne(item))
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
    qreal nodeHeight = /*(m_items.isEmpty())? 0 :*/ DEFAULT_ITEM_WIDTH;
    if(m_isExpanded)
    {
        qreal childsHeight = 0;
        foreach(GanttInfoItem* item, m_items)
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
    return m_items.indexOf(p);
}

UtcDateTime GanttInfoNode::calcDt() const
{
    return m_calcDt;
}

bool GanttInfoNode::hasCalcDt() const
{
    return m_calcDt.isValid();
}

void GanttInfoNode::setCalcDt(const UtcDateTime &calcDt)
{
    if(m_calcDt == calcDt)
        return;
    
    m_calcDt = calcDt;
    emit calcDtChanged();
}

void GanttInfoNode::callForEachItemRecursively(void (*func)(GanttInfoItem *))
{
    if(!func)
        return;

    foreach(GanttInfoItem* item, m_items)
        item->callForEachItemRecursively(func);

    (*func)(this);
}
void GanttInfoNode::setIsExpanded(bool isExpanded)
{
    m_isExpanded = isExpanded;
}

bool GanttInfoNode::isExpanded() const
{
    return m_isExpanded;
}


