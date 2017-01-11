#include "ganttinfoitem.h"
#include "ganttinfonode.h"

#include "ganttglobalvalues.h"

GanttInfoItem::GanttInfoItem(QObject *parent)
    :QObject(parent)
{
    m_linkCnt = 0;
    m_deleted = false;
    m_parent = NULL;

    connect(this,SIGNAL(indexChanged()),this,SIGNAL(changed()));
    connect(this,SIGNAL(titleChanged()),this,SIGNAL(changed()));
    connect(this,SIGNAL(parentChanged()),this,SIGNAL(changed()));
}

GanttInfoItem::~GanttInfoItem()
{
}

QString GanttInfoItem::title() const
{
    return m_title;
}

void GanttInfoItem::setTitle(const QString &title)
{
    if(title == m_title)
        return;
    m_title = title;
    emit titleChanged();
}

int GanttInfoItem::row() const
{
    if(m_parent)
        return m_parent->indexOf(this);

    return 0;
}

int GanttInfoItem::indexOf(const GanttInfoItem* p_item) const
{
    Q_UNUSED(p_item);
    return -1;
}


QModelIndex GanttInfoItem::index() const
{
    return m_index;
}

void GanttInfoItem::increaseLinkCnt()
{
    ++m_linkCnt;
}

void GanttInfoItem::reduceLinkCnt()
{
    --m_linkCnt;
    tryDeleteHimself();
}

QPair<UtcDateTime,UtcDateTime> myMax(const QPair<UtcDateTime,UtcDateTime>&f,const QPair<UtcDateTime,UtcDateTime>&s)
{
    UtcDateTime resFirst,resSecond;

    if(f.first.isValid() && s.first.isValid())
        resFirst = qMin(f.first,s.first);
    else if(f.first.isValid())
        resFirst = f.first;
    else
        resFirst = s.first;

    if(f.second.isValid() && s.second.isValid())
        resSecond = qMax(f.second,s.second);
    else if(f.second.isValid())
        resSecond = f.second;
    else
        resSecond = s.second;

    return QPair<UtcDateTime,UtcDateTime>(resFirst,resSecond);
}

QPair<UtcDateTime,UtcDateTime> GanttInfoItem::getLimits(const GanttInfoItem *root)
{
    QPair<UtcDateTime,UtcDateTime> res;
    if(!root)
        return res;
    const GanttInfoLeaf*leaf = qobject_cast<const GanttInfoLeaf*>(root);
    if(leaf)
    {
        return qMakePair(leaf->start(),leaf->finish());
    }

    const GanttInfoNode *rNode = qobject_cast<const GanttInfoNode*>(root);
    res = qMakePair(rNode->calcDt(),rNode->calcDt());
    for(int i = 0; i<rNode->size(); ++i)
    {
        res = myMax(res,getLimits(rNode->child(i)));
    }
    return res;
}

void GanttInfoItem::setIndex(const QModelIndex &index)
{
    if(index == m_index)
        return;

    m_index = index;
    emit indexChanged();
}

void GanttInfoItem::setParent(GanttInfoNode *parent)
{
    if(parent == m_parent)
        return;

    m_parent = parent;
    emit parentChanged();
}

void GanttInfoItem::deleteInfoItem()
{
    emit aboutToBeDeleted();
    tryDeleteHimself();
}

void GanttInfoItem::tryDeleteHimself()
{
    if(m_linkCnt>0)
        return;

    m_mutex.lock();
    if(!m_deleted)
    {
        this->deleteLater();
        m_deleted = true;
    }
    m_mutex.unlock();
}

qreal GanttInfoItem::pos() const
{
    GanttInfoNode * p_parent = parent();

    if(!p_parent)
        return 0.0; // m_root

    qreal base = p_parent->pos();


    if(!p_parent->isExpanded())
        return base;

    qreal offset = (p_parent->parent())?(DEFAULT_ITEM_HEIGHT):(0); // if m_root don't need extra offset
    const QList<GanttInfoItem*>& items = p_parent->m_items;
    // looking for this item, stops when found
    for(int i = 0; i < items.size(); ++i)
    {
        if( items[i] == this)
            break;

        offset += items[i]->height();
    }

    return base + offset;
}


GanttInfoNode *GanttInfoItem::parent() const
{
    return m_parent;
}

