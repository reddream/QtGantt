#ifndef INTERVALSLIDER_H
#define INTERVALSLIDER_H

#include <QPainter>
#include <QWidget>
#include "gantt-lib_global.h"
#include <QDebug>

/*!
 * \brief The IntervalSlider class
 *
 * signals: beginMoved(long long val),
 *          endMoved(long long val)
 *
 * members: long long handleH - width() of handle
 *          long long sliderV - height() of slider rectangle
 *          long long offsetV - vertical offset of [slider rectangle] in external widget
 *
 *
 *
 * Переопределяя функции drawHandle, drawSliderLine в классе-потомке можно настроить вид слайдера.
 *
 * Чтобы класс мог реагировать на сигналы от клавиатуры, необходимо установить слайдер как фильтр для
 * объекта, принимающего сигналы от клавиатуры, пример:
 *
 *  В Конструкторе MainWindow::
 *      {
 *          ...
 *          installEventFilter(ui->widgetIntervalSlider);
 *          ...
 *      }
 *
 */


class GANTTLIBSHARED_EXPORT IntervalSlider : public QWidget
{
    Q_OBJECT

    void init();
public:
    enum  ClippedHandle{
        NoHandle,
        BeginHandle,
        EndHandle
    };

    explicit IntervalSlider(QWidget *parent = 0);
    ~IntervalSlider(){}

    void setHandleSize(int new_handle_value);///< Устанавливает размер бегунков

    /** Устанавливают новые значения для бегунков */
    virtual void setEndHandle(long long endHandle, bool manually = false);
    virtual void setBeginHandle(long long beginHandle, bool manually = false);


    /** Устанавливают геометрию слайдера */
    void setSliderV(int new_sliderV);
    void setOffsetV(int new_offsetV);

    /** Устанавливает смещение в пикселях левого слайдера и правого слайдера,
     *  относительно начала/конца соответственно */
    void setLeftOffset(qreal value);
    void setRightOffset(qreal value);


    long long endHandle() const;
    long long beginHandle() const;
    long long maxValue() const;
    long long minValue() const;

    virtual void setLimits(long long minValue,long long maxValue);
    virtual void setHandles(long long beginHandle,long long endHandle, bool manually = false);

    int handleSize() const;
    int sliderV() const{return m_sliderV;}
    int intervalSliderHeight() const;

    qreal begin() const;
    qreal end() const;
    qreal valueToPos(long long val) const;

    long long sliderSize() const;

    int halfHandleSize() const;

    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    void setVisible(bool visible);

signals:
    void beginMovedManually(long long value);
    void endMovedManually(long long value);

    void beginMoved(long long value);
    void endMoved(long long value);
protected:
    void paintEvent( QPaintEvent *event );
    virtual void drawHandle( QPainter *painter,
        const QRect& handleRect, bool is_selected = false) const;
    virtual void drawSliderLine(QPainter *painter, const QRect &sliderRect ) const;

    bool eventFilter(QObject *, QEvent *);

    void mouseMoveEvent( QMouseEvent *e );
    void mouseReleaseEvent( QMouseEvent *e );
    void mousePressEvent( QMouseEvent *e );
    void mouseDoubleClickEvent(QMouseEvent *e);

    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

    void leaveEvent(QEvent *);

    bool posOverBeginHandle(const QPoint& pos) const;
    bool posOverEndHandle(const QPoint& pos) const;

private:
    /** Устанавливают область значений */
    void setMaxValue(long long maxValue);
    void setMinValue(long long minValue);

    void setBeginAndEnd(long long begin, long long end, bool manually = false);

    void setPressedAt(int pos);
    void clearPressed();


protected:
    ClippedHandle m_clippedHandle;

    long long    m_beginValue,
            m_endValue,
            m_maxValue,
            m_minValue;

    int m_handleH,
        m_sliderV,
        m_offsetV,
        m_borderWidth,
        _pressedAt;

    qreal m_leftOffset,
        m_rightOffset;

    bool m_shiftModifier,
        m_limitsSet,
        m_isHidden,
        _pressed;

protected:
    virtual bool moveHandles(long long deltaVal, bool manually = false);

    long long pointToValue(int x, ClippedHandle handle) const;
    long long pointToValue(const QPoint &p,ClippedHandle handle) const;  ///< Handles exists in the own relative coordinate system, so needs to know which system before translation
    int valueToPoint(long long value,ClippedHandle handle) const;        ///< Handles exists in the own relative coordinate system, so needs to know which system before translation

    void checkHandlesRanges();
    void checkItemRange(long long &item);
};

#endif // INTERVALSPLIDER_H

