#ifndef GANTTDTLINE_H
#define GANTTDTLINE_H

#include "dtline.h"

class GanttDtLine : public DtLine
{
    Q_OBJECT
    void init();
public:
    explicit GanttDtLine(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

signals:
    void dtChanged(const UtcDateTime &dt);
    void dtChangedManually(const UtcDateTime &dt);


public slots:
    void setCurrentDt(const UtcDateTime &dt, bool manually = false);

    void showCurrentDt(bool show);
    void showCurrentDt();
    void hideCurrentDt();
    void updateCurrentDtPath();
private:
    UtcDateTime _dt;
    QPainterPath _arrowPath;
    static QPainterPath _globalArrowPath;
    bool _drawCurrentDt;
};

#endif // GANTTDTLINE_H
