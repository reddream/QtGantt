#ifndef GANTTGRAPHICSOBJECT_H
#define GANTTGRAPHICSOBJECT_H

#include "extensions/dtline.h"
#include <QGraphicsObject>
#include <QPointer>

class GanttInfoItem;
class GanttScene;

class GanttGraphicsObject : public QGraphicsObject
{
    Q_OBJECT

    void init();
public:
    GanttGraphicsObject(GanttInfoItem *item,QGraphicsItem *parent = NULL);
    virtual ~GanttGraphicsObject();

    virtual GanttInfoItem *info() const;

    virtual void setScene(GanttScene *scene);
    void setDtLine(DtLine *dtline);

public slots:
    virtual void updateItemGeometry();

signals:
    void graphicsItemPressed();
    void graphicsItemHoverEnter();
    void graphicsItemHoverLeave();

protected:
    DtLine *_dtline;
    GanttScene *_scene;
    QPointer<GanttInfoItem> m_info;
};

#endif // GANTTGRAPHICSOBJECT_H
