#ifndef GANTTSCENE_H
#define GANTTSCENE_H


//#include "ganttheader.h"
#include "ganttcurrentdtslider.h"
//#include "gantttreemodel.h"
#include "dtline.h"

#include <QGraphicsScene>
#include <QMap>
#include <QPointer>

//class GanttGraphicsView;
class GanttIntervalGraphicsObject;
class GanttCalcGraphicsObject;
class GanttDtCrossObject;
class GanttHoverGraphicsObject;

//class QModelIndex;

#include "ganttinfotree.h"

class GanttScene : public QGraphicsScene
{
    Q_OBJECT


private:
    void init();

    DtLine *_dtline;

public:
    GanttScene(DtLine *dtline, QObject * parent = 0);

    int dtToPos(const UtcDateTime &dt) const;
    UtcDateTime posToDt(int pos) const;

signals:

public slots:


    void drawBackground(QPainter *painter, const QRectF &rect); ///< Задний план содержит сетку

    void updateWidth(int w);
    void updateHeight(int h);

    QGraphicsObject* itemByInfo(const GanttInfoItem *key) const;

    void updateSliderRect();

    void onViewAdded(QGraphicsView* view);

    UtcDateTime slidersDt() const;

    const GanttInfoLeaf *nextEvent(const UtcDateTime &curDt) const;
    const GanttInfoLeaf *prevEvent(const UtcDateTime &curDt) const;
    void removeByInfo(const GanttInfoItem* item);
    void removeByInfoLeaf(const GanttInfoLeaf* leaf);

    const QList<GanttIntervalGraphicsObject *>& dtItems() const;

    void setDrawCurrentDtSlider(bool enable);

    QGraphicsItem *currentItem() const;


    bool isVisible(const QGraphicsItem* which) const;
    QRectF elementsBoundingRect();

    void clear();
    bool sceneHaveItems() const;


signals:
    void viewResized(); //?
    void currentDtChanged(const UtcDateTime& dt);

    void graphicsItemHoverEnter(const GanttInfoItem*);
    void graphicsItemHoverLeave(const GanttInfoItem*);
    void currentItemChanged(const GanttInfoItem*);



public slots:
    void onViewResize(const QSize& newSize);
    void updateSceneRect();

    void makeStep(int step);
    void moveSliderToNextEventStart();
    void moveSliderToPrevEventFinish();
    void moveSliderToViewStart();
    void moveSliderToViewFinish();
    void moveSliderToStart();
    void setCurrentItem(QGraphicsObject *currentItem);

    QGraphicsObject *objectForPos(const QPointF& pos);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:


    void updateSlider();
    void updateItems();

private slots:
    void onVisItemDestroyed();
    void onGraphicsItemPress();
    void onGraphicsItemHoverEnter();
    void onGraphicsItemHoverLeave();

    void onInfoDelete();
    void onInfoLeafDelete();
    void onLeafStartChanged();
    void onLeafFinishChanged();

private:

    QList<GanttIntervalGraphicsObject*> m_items;
    QList<GanttCalcGraphicsObject*> m_calcItems;
    QMap<const GanttInfoItem*, QGraphicsObject*> m_itemByInfo;
    QMap<UtcDateTime,const GanttInfoLeaf*> m_infoByStart,
                                            m_infoByFinish;

    QPointer<GanttCurrentDtSlider> m_slider;
    QPointer<QGraphicsObject> m_currentItem;
    QPointer<QGraphicsView> m_view;
    QPointer<GanttDtCrossObject> m_crossObject;
    QPointer<GanttHoverGraphicsObject> m_hoverObject;
};



#endif // GANTTSCENE_H
