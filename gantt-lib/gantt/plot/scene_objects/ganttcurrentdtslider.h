#ifndef GANTTCURRENTTIMESLIDER_H
#define GANTTCURRENTTIMESLIDER_H

#include "utcdatetime.h"
#include "ganttinfonode.h"
#include "dtline.h"


#include <QGraphicsObject>
#include <QPair>

class GanttScene;

class GanttCurrentDtSlider : public QGraphicsObject
{
    Q_OBJECT

    void init();
public:
    GanttCurrentDtSlider(QGraphicsItem * parent = NULL);
    GanttCurrentDtSlider(GanttScene *scene, DtLine *dtline, QGraphicsItem *parent = NULL);

    void setScene(GanttScene* scene);
    void setDtLine(DtLine *dtline);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

    void updateShape();

    void setSlidersRect(const QRectF &slidersRect);

    void setPos(const QPointF &pos);
    void setPos(qreal x, qreal y);

    UtcDateTime dt() const;

    bool initialized() const;
    qreal relativePos() const;

    UtcDateTime minDt() const;
    UtcDateTime maxDt() const;

    bool draw() const;
    void setDraw(bool draw);

    void setVisible(bool visible);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);



signals:
    void dtChanged(UtcDateTime dt);
    void drawChanged(bool);

public slots:
    void updateTextRect();
    void setToBegin();
    void makeStep(int deltaVal);
    bool setDt(UtcDateTime dt);
    void updateScenePos();
    void updateRange(const UtcDateTime& minDt, const TimeSpan& ts);
//    void updateRange(const GanttInfoNode *node);

    void moveToBegin();
    void moveToEnd();

    void moveToRangeStart();
    void moveToRangeFinish();

private:

//    QPair<UtcDateTime,UtcDateTime> getLimits(const GanttInfoItem *root) const;
    bool outOfRange() const;
    bool outOfBounds(const UtcDateTime &dt) const;

private:
    QRectF m_slidersRect; ///< The rectangle, where slider can actually move.
    QRect m_textRect;

    QPainterPath m_sliderShape;
    QPainterPath m_rhombus;

    qreal m_penWidth;

    UtcDateTime m_dt;
    UtcDateTime m_minDt,m_maxDt;

    GanttScene* _scene;
    DtLine *_dtline;

    bool m_initialized;
    bool m_draw, m_visible;
};

#endif // GANTTCURRENTTIMESLIDER_H
