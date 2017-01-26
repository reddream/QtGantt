#ifndef GANTTINFOLEAF_H
#define GANTTINFOLEAF_H

#include "ganttinfoitem.h"

#include "utcdatetime.h"

#include <QColor>

class GanttInfoNode;

class GANTTLIBSHARED_EXPORT GanttInfoLeaf : public GanttInfoItem
{
    Q_OBJECT

public:
    GanttInfoLeaf(QObject *parent = NULL);
    GanttInfoLeaf(const QString &title
                  , const UtcDateTime   &start
                  , const TimeSpan      &ts
                  , const QModelIndex   &index
                  , const QColor        &color = Qt::green
                  , GanttInfoNode       *parentNode = NULL
                  , QObject             *parent = NULL);

    int columnCount() const;
    qreal height() const;
    void callForEachItemRecursively(void (*func)(GanttInfoItem*));
    GanttInfoNode *node();

private:


};

#endif // GANTTINFOITEM_H
