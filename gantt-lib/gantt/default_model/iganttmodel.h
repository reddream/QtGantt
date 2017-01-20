#ifndef IGANTTMODEL_H
#define IGANTTMODEL_H

#include "gantt-lib_global.h"
#include "utcdatetime.h"

#include <QModelIndex>


class GANTTLIBSHARED_EXPORT IGanttModel
{
public:
    IGanttModel();
    virtual ~IGanttModel();

    // --- Interface

    virtual QString     title(const QModelIndex &index) const = 0;
    virtual UtcDateTime start(const QModelIndex &index) const = 0;
    virtual TimeSpan    timeSpan(const QModelIndex &index) const = 0;
    virtual QColor      color(const QModelIndex &index) const = 0;

    virtual QModelIndex index(const QString &title) const = 0;     ///< QModelIndex через Title

    // ---

public:
    bool isDot(QModelIndex &index){
        return timeSpan(index).microseconds() == 0;
    }

};

#endif // IGANTTMODEL_H
