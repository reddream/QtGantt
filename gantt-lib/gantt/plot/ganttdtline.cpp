#include "ganttdtline.h"
#include "gantt-lib_global_values.h"

#include <QPainter>
#include <QMouseEvent>

QPainterPath GanttDtLine::initFunction(){
    static const int topPos = DtLine::topHeight + DtLine::bottomHeight * 1. / 3;

    QPainterPath res;
    res.moveTo(0, topPos);
    res.lineTo(4, DtLine::widgetHeight);
    res.lineTo(-4,DtLine::widgetHeight);
    res.lineTo(0, topPos);

    return res;
}

QPainterPath GanttDtLine::_globalArrowPath = GanttDtLine::initFunction();

void GanttDtLine::init()
{
    _drawCurrentDt = false;
    setMouseTracking(true);
}

GanttDtLine::GanttDtLine(QWidget *parent)
    :   DtLine(parent)
{
    init();
}

void GanttDtLine::paintEvent(QPaintEvent *e)
{
    DtLine::paintEvent(e);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    if(_drawCurrentDt){
        painter.fillPath(_arrowPath , QBrush(Qt::red));
        painter.drawPath(_arrowPath);
    }
}

void GanttDtLine::mouseMoveEvent(QMouseEvent *e)
{
    DtLine::mouseMoveEvent(e);
    if(cursor().shape() == PLAYER_CURSOR_PRESSED){
        setCurrentDt(posToDt(e->pos().x()) , true);
    }
    else{
        if(_arrowPath.contains(e->pos()))
            setCursor(PLAYER_CURSOR_OVER);
        else
            setCursor(Qt::ArrowCursor);
    }
}

void GanttDtLine::mousePressEvent(QMouseEvent *e)
{
    DtLine::mousePressEvent(e);
    if(_arrowPath.contains(e->pos()))
        setCursor(PLAYER_CURSOR_PRESSED);
}

void GanttDtLine::mouseReleaseEvent(QMouseEvent *e)
{
    DtLine::mouseReleaseEvent(e);
    setCursor(Qt::ArrowCursor);
    mouseMoveEvent(e);
}

void GanttDtLine::setCurrentDt(const UtcDateTime &dt, bool manually)
{
    if(dt == _dt)
        return; // do nothing if equal

    _dt = dt;
    updateCurrentDtPath();

    if(manually)
        emit dtChangedManually(dt);
    emit dtChanged(dt);
}

void GanttDtLine::showCurrentDt(bool show)
{
    if(show)
        showCurrentDt();
    else
        hideCurrentDt();
}

void GanttDtLine::showCurrentDt()
{
    _drawCurrentDt = true;
    update();
}

void GanttDtLine::hideCurrentDt()
{
    _drawCurrentDt = false;
    update();
}

void GanttDtLine::updateCurrentDtPath()
{
    _arrowPath = _globalArrowPath.translated(dtToPos(_dt),0);
    update();
}
