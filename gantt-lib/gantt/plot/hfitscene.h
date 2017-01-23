#ifndef HFITSCENE_H
#define HFITSCENE_H

#include <QGraphicsScene>
#include "ganttgraphicsview.h"

class HFitScene : public QGraphicsScene
{
    Q_OBJECT

    void init();
public:
    HFitScene(GanttGraphicsView *view, QObject *parent = 0);
    virtual ~HFitScene(){}
    GanttGraphicsView *view() const{
        return _view;
    }

public slots:
    virtual void onViewResized(const QSize &newSize);

protected:
    void updateWidth(int w);
    void updateHeight(int h);

protected:
    GanttGraphicsView *_view;
};

#endif // HFITSCENE_H
