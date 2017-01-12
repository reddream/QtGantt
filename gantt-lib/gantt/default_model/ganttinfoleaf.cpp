#include "ganttinfoleaf.h"
#include "gantttreemodel.h"

#include "gantt-lib_global_values.h"

GanttInfoLeaf::GanttInfoLeaf(QObject *parent)
    :GanttInfoItem(parent)
{

}

GanttInfoLeaf::GanttInfoLeaf(const QString &title, const UtcDateTime &start, const UtcDateTime &finish, const QModelIndex &index
                             , const QColor &color, GanttInfoNode *parentNode, QObject *parent)
    : GanttInfoItem(title,start,finish,index,color,parentNode,parent)
{

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



