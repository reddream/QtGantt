#ifndef GANTTSCENE_H
#define GANTTSCENE_H


#include "ganttcurrentdtslider.h"
#include "ganttdtline.h"

#include "hfitscene.h"
#include <QMap>
#include <QPointer>

//class GanttGraphicsView;
class GanttIntervalGraphicsObject;
class GanttCalcGraphicsObject;
class GanttGraphicsObject;
class GanttDtCrossObject;
class GanttHoverGraphicsObject;
class GanttInfoTree;

//class QModelIndex;

#include "ganttinfotree.h"

class GanttScene : public HFitScene
{
    Q_OBJECT

    void init();
public:
    GanttScene(GanttGraphicsView *view, GanttDtLine *dtline, QObject * parent = 0);
    int dtToPos(const UtcDateTime &dt) const;
    UtcDateTime posToDt(int pos) const;
    void setTreeInfo(GanttInfoTree *treeInfo);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);

signals:
    void limitsChanged(const UtcDateTime &min, const TimeSpan &ts);

    void scroll(int delta);
    void slideByPercent(qreal percent);
public slots:
    void onTreeInfoReset();
    void onItemAdded(GanttInfoItem *item);
    void onItemRemoved(GanttInfoItem* item);
    void onEndInsertItems();
    void onEndRemoveItems();

    void drawBackground(QPainter *painter, const QRectF &rect); ///< Задний план содержит сетку

    GanttGraphicsObject* itemByInfo(const GanttInfoItem *key) const;

    void updateSliderHeight();

    UtcDateTime slidersDt() const;

    const GanttInfoLeaf *nextEvent(const UtcDateTime &curDt) const;
    const GanttInfoLeaf *prevEvent(const UtcDateTime &curDt) const;
    void removeByInfoLeaf(const GanttInfoLeaf* leaf);

    const QList<GanttIntervalGraphicsObject *>& dtItems() const;

    void setDrawCurrentDtSlider(bool enable);

    QGraphicsItem *currentItem() const;


    bool isVisible(const QGraphicsItem* which) const;
    QRectF elementsBoundingRect();

    void clear();
    bool sceneHaveItems() const;


signals:
    void currentDtChanged(const UtcDateTime& dt);

    void graphicsItemHoverEnter(const GanttInfoItem*);
    void graphicsItemHoverLeave(const GanttInfoItem*);
    void currentItemChanged(const GanttInfoItem*);



public slots:
    void onViewResized(const QSize& newSize);
    void updateSceneRect();
    void updateSceneItems();

    void makeStep(int step);
    void moveSliderToNextEventStart();
    void moveSliderToPrevEventFinish();
    void moveSliderToViewStart();
    void moveSliderToViewFinish();
    void moveSliderToStart();
    void setCurrentByInfo(GanttInfoItem *info);
    void setCurrentItem(QGraphicsObject *currentItem);

    GanttGraphicsObject *objectForPos(const QPointF& pos);


private:
    void connectDtLine();

    void updateSlider();
    void updateItems();
    void createPersistentItems();
    void addPersistentItems();
    void removePersistentItems();
    void addInfoItem(GanttInfoItem *item);

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

    GanttInfoTree *_treeInfo;
    GanttDtLine *_dtline;
    QList<GanttIntervalGraphicsObject*> _items;
    QList<GanttCalcGraphicsObject*> _calcItems;
    QMap<const GanttInfoItem*, GanttGraphicsObject*> _itemByInfo;
    QMap<UtcDateTime,const GanttInfoLeaf*> _infoByStart,
                                            _infoByFinish;

    GanttCurrentDtSlider *_playerCurrent;
    GanttDtCrossObject *_crossObject;
    GanttHoverGraphicsObject *_hoverObject;
    QPointer<QGraphicsObject> _currentItem;
};





#endif // GANTTSCENE_H
