#include "intervalslider.h"

#include <QPainter>
#include <QStyle>
#include <QStylePainter>

#include <QKeyEvent>
#include <QStyleOption>
#include <QApplication>

#include <QDebug>


void IntervalSlider::init()
{
    _pressed = false;
    m_isHidden = false;
    m_limitsSet = false;
    m_leftOffset = m_rightOffset = 0;
    m_sliderV = 0;
    m_offsetV = 0;
    m_borderWidth = 1;

    setOffsetV(7);
    setHandleSize(12);
    setSliderV(8);


    m_maxValue=1000;
    m_minValue=0;

    m_beginValue=m_minValue;
    m_endValue=m_maxValue;

    m_clippedHandle = NoHandle;
    m_shiftModifier = false;

    connect(this,SIGNAL(beginMoved(long long)),this,SLOT(update()));
    connect(this,SIGNAL(endMoved(long long)),this,SLOT(update()));
}

IntervalSlider::IntervalSlider(QWidget *parent) :
    QWidget(parent)
{
    init();

}
long long IntervalSlider::endHandle() const
{
    return m_endValue;
}

void IntervalSlider::setEndHandle(long long endValue, bool manually)
{
    if(m_endValue==endValue)
        return;

    m_endValue = endValue;

    if(m_endValue>m_maxValue)
    {
        m_endValue=m_maxValue;
    }
    if(m_endValue<m_beginValue)
    {
        m_endValue=m_beginValue;
    }

    if(manually)
        emit endMovedManually(m_endValue);
    emit endMoved(m_endValue);
    update();
}
long long IntervalSlider::beginHandle() const
{
    return m_beginValue;
}

void IntervalSlider::setBeginHandle(long long beginValue, bool manually)
{
    if(m_beginValue==beginValue)
        return;

    m_beginValue = beginValue;

    if(m_beginValue<m_minValue)
    {
        m_beginValue=m_minValue;
    }

    if(m_endValue<m_beginValue)
    {
        m_beginValue=m_endValue;
    }

    if(manually)
        emit beginMovedManually(m_beginValue);
    emit beginMoved(m_beginValue);
    update();
}
long long IntervalSlider::maxValue() const
{
    return m_maxValue;
}

void IntervalSlider::setMaxValue(long long maxValue)
{
    if(m_maxValue == maxValue)
        return;
    m_maxValue = maxValue;
    checkHandlesRanges();
    update();
}

long long IntervalSlider::minValue() const
{
    return m_minValue;
}

void IntervalSlider::setMinValue(long long minValue)
{
    if(m_minValue == minValue)
        return;

    m_minValue = minValue;
    checkHandlesRanges();
    update();
}

void IntervalSlider::setBeginAndEnd(long long begin, long long end, bool manually)
{
    if(begin > end)
        return;

    setBeginHandle(m_minValue, manually);
    setEndHandle(m_maxValue, manually);

    setBeginHandle(begin, manually);
    setEndHandle(end, manually);

}

void IntervalSlider::setPressedAt(int pos)
{
    _pressed = true;
    _pressedAt = pos;
}

void IntervalSlider::clearPressed()
{
    _pressed = false;
    _pressedAt = -1;
}


void IntervalSlider::setLimits(long long minValue, long long maxValue)
{
    if(!m_limitsSet)
    {
        m_limitsSet=true;
        if(!m_isHidden)
            show();
    }

    setMinValue(minValue);
    setMaxValue(maxValue);

    setBeginAndEnd(minValue,maxValue,true);
}

void IntervalSlider::setHandles(long long beginHandle, long long endHandle, bool manually)
{
    if(m_clippedHandle == BeginHandle)
    {
        setBeginHandle(beginHandle,manually);
        setEndHandle(endHandle,manually);
    }
    else if(m_clippedHandle == EndHandle)
    {
        setEndHandle(endHandle,manually);
        setBeginHandle(beginHandle,manually);
    }
}

int IntervalSlider::handleSize() const
{
    return m_handleH + 2 ; // 1 - borderwidth
}

void IntervalSlider::setHandleSize(int new_handle_value)
{
    m_handleH = new_handle_value;
    setMinimumWidth(2*handleSize());
    update();
}

void IntervalSlider::setSliderV(int new_sliderV)
{
    if(m_sliderV == new_sliderV)
        return;
    m_sliderV = new_sliderV;


    setMinimumHeight(intervalSliderHeight());
    setMaximumHeight(intervalSliderHeight());

    updateGeometry();
}

void IntervalSlider::setOffsetV(int new_offsetV)
{
    if(m_offsetV == new_offsetV)
        return;
    m_offsetV=new_offsetV;

    setMinimumHeight(intervalSliderHeight());
    setMaximumHeight(intervalSliderHeight());

    updateGeometry();
}




long long IntervalSlider::pointToValue(const QPoint &p,ClippedHandle handle) const
{
    return pointToValue(p.x(),handle);
}

int IntervalSlider::valueToPoint(long long value,ClippedHandle handle) const
{
    long long relativeValue = value-m_minValue;
    int offset= halfHandleSize() + m_leftOffset;
    long long widthDiff = 2*handleSize() + m_rightOffset + m_leftOffset /*+((handle==NoHandle)?(0):(handleSize()))*/,
         relWidth= width() - widthDiff;

    switch(handle)
    {
    case BeginHandle:
        break;
    case EndHandle:
        offset+=handleSize();
        break;
    case NoHandle:
    {
        qreal x = (value - beginHandle())* 1.0/(endHandle() - beginHandle());
        offset+=qMin(qMax(x,0.0),1.0) * handleSize();
        break;
    }
    default:
        break;
    }

    int relPoint = (m_maxValue-m_minValue)?
            (relativeValue*1.0/(m_maxValue-m_minValue) * relWidth + 0.5)
              :((handle == EndHandle)?relWidth:0);
    if(relPoint<0)
        relPoint=0;
    if(relPoint>relWidth)
        relPoint=relWidth;

    return offset+relPoint;
}

void IntervalSlider::checkHandlesRanges()
{
    checkItemRange(m_beginValue);
    checkItemRange(m_endValue);
}

void IntervalSlider::checkItemRange(long long &itemVal)
{
    if(itemVal<m_minValue)
        itemVal = m_minValue;
    if(itemVal>m_maxValue)
        itemVal=m_maxValue;
}

void IntervalSlider::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter( this );

    drawSliderLine( &painter, rect());
    drawHandle( &painter, getBeginHandleRect(),
                (m_clippedHandle == BeginHandle || m_shiftModifier ) );
    drawHandle( &painter, getEndHandleRect(),
                (m_clippedHandle == EndHandle || m_shiftModifier ) );
}

void IntervalSlider::drawHandle(QPainter *painter, const QRect& handleRect, bool is_selected) const
{
    QPalette::ColorRole handleRole,handleLineRole;
    QColor color;

    if(is_selected || (QApplication::keyboardModifiers()&Qt::ShiftModifier))
    {
        handleRole = QPalette::Mid;
        handleLineRole = QPalette::Button;
    }
    else
    {
        handleRole = QPalette::Button;
        handleLineRole = QPalette::Mid;
    }

    int borderWidth = 1;

    painter->setPen(palette().color(handleLineRole));
    painter->setBrush(palette().color(handleRole));

    qDrawShadePanel( painter,
                     handleRect, palette(), false, borderWidth,
                     &palette().brush(handleRole) );

    int lineX = handleRect.center().x();
    QLineF handleMidLine(lineX, handleRect.top()+borderWidth,
                         lineX, handleRect.bottom()-borderWidth);


    painter->drawLine(handleMidLine);
}


void IntervalSlider::drawSliderLine(QPainter *painter, const QRect &sliderRect) const
{
    int sliderV = m_sliderV,
        sliderH = sliderRect.width();

    QRect rSlot( sliderRect.x() + halfHandleSize(),
                   sliderRect.y() + (m_offsetV>0?m_offsetV:0),
                   sliderH - 2 * halfHandleSize(), sliderV );
    QBrush brush = palette().brush( QPalette::Dark );

    qDrawShadePanel( painter, rSlot, palette(), true, m_borderWidth , &brush );
}

bool IntervalSlider::eventFilter(QObject *object, QEvent *p_event)
{
    if(p_event->type() == QEvent::KeyPress || p_event->type() == QEvent::KeyRelease)
        event(p_event);

    return QWidget::eventFilter(object,p_event);
}

void IntervalSlider::keyPressEvent(QKeyEvent *e)
{
    if(e->modifiers() & Qt::ShiftModifier)
    {
        m_shiftModifier = true;
        checkForCursor(mapFromGlobal(QCursor::pos()));
        update();
    }

    QWidget::keyPressEvent(e);
}

void IntervalSlider::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Shift)
    {
        m_shiftModifier = false;
        checkForCursor(mapFromGlobal(QCursor::pos()));
        update();
    }

    QWidget::keyReleaseEvent(e);
}

void IntervalSlider::leaveEvent(QEvent *)
{
    update();
}


bool IntervalSlider::posOverBeginHandle(const QPoint& pos) const
{
    int beginSpliter1 = valueToPoint(m_beginValue,BeginHandle)-halfHandleSize(),
        beginSpliter2 = beginSpliter1 + handleSize();

    return (pos.x()>beginSpliter1 && pos.x()<beginSpliter2);
}

bool IntervalSlider::posOverEndHandle(const QPoint& pos) const
{
    int endSpliter1 = valueToPoint(m_endValue,EndHandle)-halfHandleSize(),
         endSpliter2 = endSpliter1 + handleSize();

    return (pos.x()>endSpliter1 && pos.x()<endSpliter2);
}


void IntervalSlider::setRightOffset(qreal value)
{
    m_rightOffset = value;
}

void IntervalSlider::setLeftOffset(qreal value)
{
    m_leftOffset = value;
}

int IntervalSlider::intervalSliderHeight() const
{
    return qMax(m_sliderV+2*(m_offsetV+m_borderWidth), m_sliderV+m_borderWidth);
}

qreal IntervalSlider::begin() const
{
    return (qreal)(m_beginValue - m_minValue)/ sliderSize();
}

qreal IntervalSlider::end() const
{
    return (qreal)(m_endValue - m_minValue)/ sliderSize();
}

qreal IntervalSlider::valueToPos(long long val) const
{
    return (qreal)(val - m_minValue)/ sliderSize();
}

long long IntervalSlider::sliderSize() const
{
    return m_maxValue - m_minValue;
}

int IntervalSlider::halfHandleSize() const {return handleSize()/2;}

QSize IntervalSlider::sizeHint() const
{
    return QSize(200, intervalSliderHeight());
}

QSize IntervalSlider::minimumSizeHint() const
{
    return QSize(200, intervalSliderHeight());
}

void IntervalSlider::setVisible(bool visible)
{
    m_isHidden=!visible;
    if(!m_limitsSet&&visible)
        return;
    QWidget::setVisible(visible);
}

void IntervalSlider::checkForCursor(const QPoint &pos)
{
    if(m_shiftModifier ||
            getBeginHandleRect().contains(pos)||
            getEndHandleRect().contains(pos) ){
        setCursor(Qt::PointingHandCursor);
    }
    else{
        setCursor(Qt::ArrowCursor);
    }
}

QRect IntervalSlider::getBeginHandleRect() const
{
    return QRect( valueToPoint(m_beginValue,BeginHandle) - halfHandleSize(),
                    rect().y() /*+ m_borderWidth*/ - (m_offsetV<0?m_offsetV:0),
                    handleSize(),
                    rect().height() - 2*m_borderWidth + 2*(m_offsetV<0?m_offsetV:0) );
}

QRect IntervalSlider::getEndHandleRect() const
{
    return QRect( valueToPoint(m_endValue,EndHandle) - halfHandleSize(),
                    rect().y()/*+m_borderWidth*/ - (m_offsetV<0?m_offsetV:0),
                    handleSize(),
                    rect().height() - 2*m_borderWidth + 2*(m_offsetV<0?m_offsetV:0) );
}


bool IntervalSlider::moveHandles(long long deltaVal,bool manually)
{
    if(!deltaVal)
        return false;

    blockSignals(true);

    if(deltaVal>0)
    {
        setEndHandle(endHandle()+deltaVal);
        setBeginHandle(beginHandle()+deltaVal);
    }
    else
    {
        setBeginHandle(beginHandle()+deltaVal);
        setEndHandle(endHandle()+deltaVal);
    }

    blockSignals(false);

    if(manually){
        emit beginMovedManually(beginHandle());
        emit endMovedManually(endHandle());
    }
    emit beginMoved(beginHandle());
    emit endMoved(endHandle());

    return true;
}

long long IntervalSlider::pointToValue(int arg_x, IntervalSlider::ClippedHandle handle) const
{
    int x = arg_x - halfHandleSize();
    int offset = m_leftOffset;
    long long widthDiff = handleSize() +((handle==NoHandle)?(0):(handleSize())) + m_rightOffset + m_leftOffset,
         relWidth= width() - widthDiff;
    switch(handle)
    {
    case EndHandle:
        offset += handleSize();
        break;
    default:
        offset += 0;
        break;
    }
    long long relValue =(m_maxValue-m_minValue)?
            (((1.0*x-offset) / relWidth * (m_maxValue-m_minValue))+0.5)
              :(0);
    if (relValue<0)
        relValue=0;
    if(relValue>m_maxValue-m_minValue)
        relValue=m_maxValue-m_minValue;
    return m_minValue+relValue;
}

void IntervalSlider::mouseMoveEvent(QMouseEvent *e)
{
    long long val,deltaVal;

    if(_pressed){
        val = pointToValue(e->pos(), NoHandle);

        deltaVal = val - pointToValue(_pressedAt, NoHandle);
        _pressedAt = e->pos().x();

        if(endHandle() + deltaVal > m_maxValue)
            deltaVal = m_maxValue - endHandle();
        if(beginHandle() + deltaVal < m_minValue)
            deltaVal = - beginHandle() + m_minValue;

        moveHandles(deltaVal,true);
    }
    else{
        checkForCursor(e->pos());
    }

    if(m_clippedHandle==BeginHandle)
    {
        val = pointToValue(e->pos(),BeginHandle);

        setBeginHandle(val,true);
    }
    else if(m_clippedHandle==EndHandle)
    {
        val = pointToValue(e->pos(),EndHandle);

        setEndHandle(val,true);
    }
    update();
    QWidget::mouseMoveEvent(e);
}

void IntervalSlider::mouseReleaseEvent(QMouseEvent *e)
{
    clearPressed();
    m_clippedHandle=NoHandle;
    mouseMoveEvent(e);
    update();
    QWidget::mouseReleaseEvent(e);
}

void IntervalSlider::mousePressEvent(QMouseEvent *e)
{
    const QPoint &p = e->pos();

    if(m_shiftModifier)
        setPressedAt(e->pos().x());

    if(posOverBeginHandle(p))
        m_clippedHandle = BeginHandle;
    else if (posOverEndHandle(p))
        m_clippedHandle = EndHandle;
    else
        m_clippedHandle=NoHandle;

    mouseMoveEvent(e);
    QWidget::mousePressEvent(e);
}

void IntervalSlider::mouseDoubleClickEvent(QMouseEvent *e)
{
    setBeginAndEnd(m_minValue, m_maxValue, true);
    QWidget::mouseDoubleClickEvent(e);
}
