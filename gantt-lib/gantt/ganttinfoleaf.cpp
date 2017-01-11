#include "ganttinfoleaf.h"
#include "gantttreemodel.h"

#include "ganttglobalvalues.h"

GanttInfoLeaf::GanttInfoLeaf(QObject *parent)
    :GanttInfoItem(parent)
{
    m_color = Qt::green;
    m_start = m_finish = UtcDateTime();


    connect(this,SIGNAL(startChanged()),this,SIGNAL(changed()));
    connect(this,SIGNAL(finishChanged()),this,SIGNAL(changed()));
    connect(this,SIGNAL(colorChanged()),this,SIGNAL(changed()));
}


UtcDateTime GanttInfoLeaf::start() const
{
    return m_start;
}

void GanttInfoLeaf::setStart(const UtcDateTime &start)
{
    if(start == m_start)
        return;
//    UtcDateTime lastStart = m_start;
    m_start = start;
    emit startChanged(/*lastStart*/);
}
UtcDateTime GanttInfoLeaf::finish() const
{
    return m_finish;
}

void GanttInfoLeaf::setFinish(const UtcDateTime &finish)
{
    if(finish == m_finish)
        return;

//    UtcDateTime lastFinish = m_finish;
    m_finish = finish;
    emit finishChanged(/*lastFinish*/);

}
QColor GanttInfoLeaf::getColor() const
{
    return m_color;
}

void GanttInfoLeaf::setColor(const QColor &value)
{
    if(value == m_color)
        return;

    m_color = value;
    emit colorChanged();
}

long long GanttInfoLeaf::duration() const
{
    return m_start.microsecondsTo(m_finish);
}

int GanttInfoLeaf::columnCount() const
{
    return GanttTreeModel::Fields_count;
}

qreal GanttInfoLeaf::height() const
{
    return DEFAULT_ITEM_WIDTH;
}

void GanttInfoLeaf::callForEachItemRecursively(void (*func)(GanttInfoItem *))
{
    if(func)
        (*func)(this);
}



