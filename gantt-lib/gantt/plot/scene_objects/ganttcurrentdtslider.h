#ifndef GANTTCURRENTTIMESLIDER_H
#define GANTTCURRENTTIMESLIDER_H

#include "utcdatetime.h"
#include "gantt/info/ganttinfonode.h"
#include "extensions/dtline.h"


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

    int height() const;
    void setHeight(int height);

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

    void moveToBegin();
    void moveToEnd();

    void moveToRangeStart();
    void moveToRangeFinish();

private:
    bool outOfRange() const;
    bool outOfBounds(const UtcDateTime &dt) const;
    void initPath();


private:
    QPainterPath _arrowPath;
    QRect m_textRect;
    int _height;

    UtcDateTime m_dt;
    UtcDateTime m_minDt,m_maxDt;

    GanttScene* _scene;
    DtLine *_dtline;

    bool _initialized;
    bool _draw, _visible;
};

#endif // GANTTCURRENTTIMESLIDER_H
