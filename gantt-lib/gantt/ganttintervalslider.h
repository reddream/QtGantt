#ifndef GANTTINTERVALSLIDER_H
#define GANTTINTERVALSLIDER_H

#include "idtinterval.h"
#include "intervalslider.h"
#include "utcdatetime.h"

class GanttWidget;

class GanttIntervalSlider : public IntervalSlider, public IDtInterval
{
    Q_OBJECT

    void init();
public:
    explicit GanttIntervalSlider(QWidget *parent = 0);


    void setCurrentTimeRectWidth(const qreal &currentTimeRectWidth);
    void setCurrentTimeRectColor(const QColor &currentTimeRectColor);

    UtcDateTime valToDt(long long val) const;
    long long dtToVal(const UtcDateTime& dt) const;

    UtcDateTime beginDt() const;
    UtcDateTime endDt() const;

    long long minTimeSize() const;

    void setBeginHandle(long long beginHandle);
    void setEndHandle(long long endHandle);
    void reset();

    //--- IDtInterval impl
    virtual UtcDateTime left() const;
    virtual UtcDateTime right() const;
    virtual TimeSpan timeSpan() const;
    virtual UtcDateTime min() const;
    virtual UtcDateTime max() const;

    virtual void setLeft(const UtcDateTime &dt);
    virtual void setRight(const UtcDateTime &dt);
    virtual void setMin(const UtcDateTime &dt);
    virtual void setTimeSpan(const TimeSpan &ts);


    //---
signals:
    void rangeChanged(const UtcDateTime &min, const TimeSpan &ts);
    void rangeChangedManually(const UtcDateTime &min, const TimeSpan &ts);

public slots:
    void setLimits(long long min, long long max);
    void setLimits(const UtcDateTime &min, const TimeSpan &ts);
    void setCurrentTime(const UtcDateTime &dt);
    void setDrawCurrentDt(bool draw);
    void setMinTimeSize(long long minTimeSize);

protected:
    void drawHandle(QPainter *painter, const QRect &handleRect, bool is_selected) const;
    void drawSliderLine(QPainter *painter, const QRect &sliderRect) const;

    void drawCurrentTime(QPainter *painter, const QRect &sliderRect) const;


private:
    bool outOfLimits(const UtcDateTime& dt) const;
private slots:
    void emitRangeChanged();
    void emitRangeChangedManually();

private:
    UtcDateTime m_currentTime;

    qreal m_currentTimeRectWidth;
    QColor m_currentTimeRectColor;

    long long m_shiftRange;
    long long m_minTimeSize;
    QPoint m_lastPos;
    bool m_drawCurrentDt;
};

#endif // GANTTSLIDER_H
