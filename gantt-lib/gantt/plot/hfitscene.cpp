#include "hfitscene.h"
#include "gantt-lib_global_values.h"


HFitScene::HFitScene(GanttGraphicsView *view, QObject *parent)
    : QGraphicsScene(parent)
{
    if(!(_view = view)){
        qWarning("HFitScene initialized with NULL view");
    }
    else {
        connect(_view,SIGNAL(viewResized(QSize)),this,SLOT(onViewResized(QSize)));
        _view->setScene(this);
    }
    init();
}

void HFitScene::onViewResized(const QSize &newSize)
{
    updateWidth(newSize.width());
}

void HFitScene::updateWidth(int w)
{
    if(w < GANTTSCENE_MIN_WIDTH)
        w = GANTTSCENE_MIN_WIDTH;

    setSceneRect(0,0,w,sceneRect().height());
}

void HFitScene::updateHeight(int h)
{
    setSceneRect(0,0,sceneRect().width(),h);
}

void HFitScene::init()
{

}
