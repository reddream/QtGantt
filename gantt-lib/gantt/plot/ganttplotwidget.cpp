#include "ganttplotwidget.h"

GanttPlotWidget::GanttPlotWidget(QWidget *parent)
    : QWidget(parent)
{
    init();
}

int GanttPlotWidget::dtToPos(const UtcDateTime &dt) const{
    return _dtline->dtToPos(dt);
}

UtcDateTime GanttPlotWidget::dtToPos(int pos) const{
    return _dtline->posToDt(pos);
}

void GanttPlotWidget::setMin(const UtcDateTime &dt){
    _dtline->setMin(dt);
    emit rangeChanged();
}

void GanttPlotWidget::setTimeSpan(const TimeSpan &ts){
    _dtline->setTimeSpan(ts);
    emit rangeChanged();
}

void GanttPlotWidget::setRange(const UtcDateTime &min, const UtcDateTime &max){
    if(min > max){
        qWarning("GanttPlotWidget::setRange:: min > max");
        return;
    }
    setRange(min, TimeSpan(min.microsecondsTo(max)));
}

void GanttPlotWidget::setRange(const UtcDateTime &dt, const TimeSpan &ts){
    _dtline->setMin(dt);
    _dtline->setTimeSpan(ts);
    emit rangeChanged();
}


void GanttPlotWidget::init()
{
    _scene = new GanttScene(this);
    _dtline = new DtLine(this);
}


