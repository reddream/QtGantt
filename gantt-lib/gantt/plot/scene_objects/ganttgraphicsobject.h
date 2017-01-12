#ifndef GANTTGRAPHICSOBJECT_H
#define GANTTGRAPHICSOBJECT_H

#include "dtline.h"
#include <QGraphicsObject>
#include <QPointer>

class GanttInfoItem;
class GanttScene;

class GanttGraphicsObject : public QGraphicsObject
{
public:
    GanttGraphicsObject(GanttInfoItem *item,QGraphicsItem *parent = NULL);

    virtual GanttInfoItem *info() const;

    virtual void setScene(GanttScene *scene);
    void setDtLine(DtLine *dtline){
        _dtline = dtline;
    }

protected:
    DtLine *_dtline;
    GanttScene *m_scene;
    QPointer<GanttInfoItem> m_info;
};

#endif // GANTTGRAPHICSOBJECT_H
