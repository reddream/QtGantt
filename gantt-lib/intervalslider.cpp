#include "intervalslider.h"

#include <QPainter>
#include <QStyle>
#include <QStylePainter>

#include <QKeyEvent>
#include <QStyleOption>
#include <QApplication>

#include <QDebug>


IntervalSlider::IntervalSlider(QWidget *parent) :
    QWidget(parent)
{
    hide();
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

    connect(this,SIGNAL(valueChanged(IntervalSlider::ClippedHandle,long long)),this,SLOT(update()));
}
long long IntervalSlider::endHandle() const
{
    return m_endValue;
}

void IntervalSlider::setEndHandle(long long endValue)
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

    emit valueChanged(IntervalSlider::EndHandle,m_endValue);
    emit endMoved(m_endValue);
    update();
}
long long IntervalSlider::beginHandle() const
{
    return m_beginValue;
}

void IntervalSlider::setBeginHandle(long long beginValue)
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

    emit valueChanged(IntervalSlider::BeginHandle,m_beginValue);
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

void IntervalSlider::setBeginAndEnd(long long begin, long long end)
{
    if(begin> end)
        return;

//    if(begin<m_minValue)
//        begin = m_minValue;
//    if(begin>m_maxValue)
//        begin = m_maxValue;

//    if(end<m_minValue)
//        end = m_minValue;
//    if(end>m_maxValue)
//        end = m_maxValue;

    setBeginHandle(m_minValue);
    setEndHandle(m_maxValue);

    setBeginHandle(begin);
    setEndHandle(end);

}

void IntervalSlider::setLimits(long long minValue,long long maxValue)
{
    if(!m_limitsSet)
    {
        m_limitsSet=true;
        if(!m_isHidden)
            show();
    }

    setMinValue(minValue);
    setMaxValue(maxValue);

    setBeginAndEnd(minValue,maxValue);
}

void IntervalSlider::setHandles(long long beginHandle, long long endHandle)
{
    if(m_clippedHandle == BeginHandle)
    {
        setBeginHandle(beginHandle);
        setEndHandle(endHandle);
    }
    else if(m_clippedHandle == EndHandle)
    {
        setEndHandle(endHandle);
        setBeginHandle(beginHandle);
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
    int x = p.x() - halfHandleSize();
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
//    painter.setClipRegion( event->region() );

    QStyleOption opt;
    opt.init(this);

    QRect sliderRect = opt.rect/*.adjusted(1,1,-1,-1)*/;

    drawSliderLine( &painter, sliderRect );

    QRect beginHandleRect(
                valueToPoint(m_beginValue,BeginHandle) - halfHandleSize(),
                sliderRect.y(),
                handleSize(),
                sliderRect.height()
                )
        , endHandleRect(
                valueToPoint(m_endValue,EndHandle) - halfHandleSize(),
                sliderRect.y(),
                handleSize(),
                sliderRect.height()
                );


    drawHandle( &painter, beginHandleRect,
                (m_clippedHandle == BeginHandle || m_shiftModifier ) );
    drawHandle( &painter, endHandleRect,
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
                   sliderRect.y() + m_offsetV,
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
        update();
    }

    QWidget::keyPressEvent(e);
}

void IntervalSlider::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Shift)
    {
        m_shiftModifier = false;
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
    return m_sliderV+2*(m_offsetV+m_borderWidth);
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


bool IntervalSlider::moveHandles(long long deltaVal)
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

    emit beginMoved(beginHandle());
    emit valueChanged(BeginHandle , beginHandle());
    emit endMoved(endHandle());
    emit valueChanged(EndHandle , endHandle());

    return true;
}

void IntervalSlider::mouseMoveEvent(QMouseEvent *e)
{
    long long val,deltaVal;

    if(m_clippedHandle==BeginHandle)
    {
        val = pointToValue(e->pos(),BeginHandle);
        if(m_shiftModifier)
        {
            deltaVal = val-beginHandle();
            if(endHandle()+deltaVal>m_maxValue)
            {
                deltaVal=m_maxValue-endHandle();
            }
            moveHandles(deltaVal);
        }
        else
            setBeginHandle(val);
    }
    else if(m_clippedHandle==EndHandle)
    {
        val = pointToValue(e->pos(),EndHandle);
        if(m_shiftModifier)
        {
            deltaVal = val-endHandle();
            if(beginHandle()+deltaVal<m_minValue)
            {
                deltaVal=-beginHandle()+m_minValue;
            }
            moveHandles(deltaVal);
        }
        else
            setEndHandle(val);
    }
    update();
}

void IntervalSlider::mouseReleaseEvent(QMouseEvent *e)
{
    m_clippedHandle=NoHandle;
    mouseMoveEvent(e);
    update();
}

void IntervalSlider::mousePressEvent(QMouseEvent *e)
{
    const QPoint &p = e->pos();

    if(posOverBeginHandle(p))
        m_clippedHandle = BeginHandle;
    else if (posOverEndHandle(p))
        m_clippedHandle = EndHandle;
    else
        m_clippedHandle=NoHandle;

    mouseMoveEvent(e);
}
