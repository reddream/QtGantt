#include "ganttinfoleaf.h"
#include "ganttinfonode.h"

#include "extensions/gantt-lib_global_values.h"

GanttInfoLeaf::GanttInfoLeaf(QObject *parent)
    :GanttInfoItem(parent)
{

}

GanttInfoLeaf::GanttInfoLeaf(const QString &title, const UtcDateTime &start, const TimeSpan &ts, const QModelIndex &index
                             , const QColor &color, GanttInfoNode *parentNode, QObject *parent)
    : GanttInfoItem(title,start,ts,index,color,parentNode,parent)
{

}




int GanttInfoLeaf::columnCount() const
{
    return 4;
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

GanttInfoNode *GanttInfoLeaf::node()
{
    if(parent())
        return parent()->node();
    return NULL;
}



