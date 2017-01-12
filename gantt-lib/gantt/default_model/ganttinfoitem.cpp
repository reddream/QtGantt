#include "ganttinfoitem.h"
#include "ganttinfonode.h"

#include "gantt-lib_global_values.h"

GanttInfoItem::GanttInfoItem(QObject *parent)
    : QObject(parent)
{
    init();
}

GanttInfoItem::GanttInfoItem(const QString &title, const UtcDateTime &start, const UtcDateTime &finish
                             , const QModelIndex &index
                             , const QColor &color
                             , GanttInfoNode *parentNode
                             , QObject *parent)
    : QObject(parent)
{
    init();

    setTitle(title);
    setStart(start);
    setFinish(finish);
    setIndex(index);
    setParent(parentNode);
    setColor(color);
}

GanttInfoItem::~GanttInfoItem()
{
}

QString GanttInfoItem::title() const
{
    return _title;
}

void GanttInfoItem::setTitle(const QString &title)
{
    if(title == _title)
        return;
    _title = title;
    emit titleChanged();
}

int GanttInfoItem::row() const
{
    if(_parent)
        return _parent->indexOf(this);

    return 0;
}

int GanttInfoItem::indexOf(const GanttInfoItem* p_item) const
{
    Q_UNUSED(p_item);
    return -1;
}

void GanttInfoItem::init()
{
    _linkCnt = 0;
    _deleted = false;
    _parent = NULL;
    _color = Qt::green;
    _start = _finish = UtcDateTime();
    _index = QModelIndex();

    connect(this,SIGNAL(indexChanged()),this,SIGNAL(changed()));
    connect(this,SIGNAL(titleChanged()),this,SIGNAL(changed()));
    connect(this,SIGNAL(parentChanged()),this,SIGNAL(changed()));
    connect(this,SIGNAL(startChanged()),this,SIGNAL(changed()));
    connect(this,SIGNAL(finishChanged()),this,SIGNAL(changed()));
    connect(this,SIGNAL(colorChanged()),this,SIGNAL(changed()));
}

QColor GanttInfoItem::color() const
{
    return _color;
}

bool GanttInfoItem::hasStart() const
{
    return _start.isValid();
}

bool GanttInfoItem::hasFinish() const
{
    return _finish.isValid();
}

long long GanttInfoItem::duration() const
{
    return _start.microsecondsTo(_finish);
}

void GanttInfoItem::setColor(const QColor &color)
{
    if(color == _color)
        return;
    _color = color;
    emit colorChanged();
}

UtcDateTime GanttInfoItem::finish() const
{
    return _finish;
}

void GanttInfoItem::setFinish(const UtcDateTime &finish)
{
    if(finish == _finish)
        return;
    _finish = finish;
    emit startChanged();
}

UtcDateTime GanttInfoItem::start() const
{
    return _start;
}

void GanttInfoItem::setStart(const UtcDateTime &start)
{
    if(start == _start)
        return;
    _start = start;
    emit startChanged();
}


QModelIndex GanttInfoItem::index() const
{
    return _index;
}

void GanttInfoItem::increaseLinkCnt()
{
    ++_linkCnt;
}

void GanttInfoItem::reduceLinkCnt()
{
    --_linkCnt;
    tryDelete();
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
    res = qMakePair(root->start(),root->finish());

    const GanttInfoLeaf*leaf = qobject_cast<const GanttInfoLeaf*>(root);
    if(leaf)
    {
        return res;
    }
    const GanttInfoNode *rNode = qobject_cast<const GanttInfoNode*>(root);
    for(int i = 0; i<rNode->size(); ++i)
    {
        res = myMax(res,getLimits(rNode->at(i)));
    }
    return res;
}

bool GanttInfoItem::operator==(const GanttInfoItem &another)
{
    return _title == another._title;
}

void GanttInfoItem::setIndex(const QModelIndex &index)
{
    if(index == _index)
        return;

    _index = index;
    emit indexChanged();
}

void GanttInfoItem::setParent(GanttInfoNode *parent)
{
    if(parent == _parent)
        return;

    _parent = parent;
    emit parentChanged();
}

void GanttInfoItem::deleteInfoItem()
{
    emit aboutToBeDeleted();
    tryDelete();
}

void GanttInfoItem::tryDelete()
{
    if(_linkCnt>0)
        return;

    _mutex.lock();
    if(!_deleted)
    {
        this->deleteLater();
        _deleted = true;
    }
    _mutex.unlock();
}

qreal GanttInfoItem::pos() const
{
    GanttInfoNode * p_parent = parent();

    if(!p_parent)
        return 0.0; // m_root

    qreal base = p_parent->pos();


    if(!p_parent->expanded())
        return base;

    qreal offset = (p_parent->parent())?(DEFAULT_ITEM_HEIGHT):(0); // if root needn't extra offset
    const QList<GanttInfoItem*>& items = p_parent->_items;
    // looking for this item, stops on found
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
    return _parent;
}

