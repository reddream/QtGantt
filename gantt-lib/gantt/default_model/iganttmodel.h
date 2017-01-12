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

    virtual QString     title(const QModelIndex &index) = 0;
    virtual UtcDateTime start(const QModelIndex &index) = 0;
    virtual UtcDateTime finish(const QModelIndex &index) = 0;
    virtual QColor      color(const QModelIndex &index) = 0;

    virtual QModelIndex index(const QString &title) = 0;     ///< QModelIndex через Title

    // ---

public:
    bool hasFinish(QModelIndex &index){
        return finish(index).isValid();
    }

};

#endif // IGANTTMODEL_H
