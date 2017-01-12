#ifndef GANTTPLOTWIDGET_H
#define GANTTPLOTWIDGET_H

#include "dtline.h"
#include "ganttscene.h"
#include "ganttinfotree.h"


#include <QWidget>

class GanttPlotWidget : public QWidget
{
    Q_OBJECT
public:
    GanttPlotWidget(QWidget *parent = 0);

    inline int dtToPos(const UtcDateTime &dt) const;
    inline UtcDateTime dtToPos(int pos) const;


signals:
    void rangeChanged(); /// TODO connect to SceneSomething

public slots:
    void setMin(const UtcDateTime &dt);
    void setTimeSpan(const TimeSpan &ts);
    void setRange(const UtcDateTime &min, const UtcDateTime &max);
    void setRange(const UtcDateTime &dt, const TimeSpan &ts);


private:
    void init();

    GanttScene *_scene;
    DtLine *_dtline;
};

#endif // GANTTPLOTWIDGET_H
