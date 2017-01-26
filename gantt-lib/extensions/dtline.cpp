#include "dtline.h"
#include "gantt-lib_global_values.h"

#include <QPainter>

const TimeSpan DtLine::_minTimeSpan(30 * _MICROSECONDS_IN_SECOND);

enum defaults {
    itemHeight = 20,
    heightConstraint = 2 * itemHeight,
};

DtLine::Precision DtLine::greaterPrecision(DtLine::Precision mode)
{
    int res = mode;
    ++res;
    if(res >= (int)Precision_count){
        qWarning("Precision::greaterPrecision out of range");
        return minMode();
    }
    return static_cast<Precision>(res);
}

DtLine::Precision DtLine::lessPrecision(DtLine::Precision mode)
{
    int res = mode;
    --res;
    if(res < 0){
        qWarning("Precision::lessPrecision out of range");
        return maxMode();
    }
    return static_cast<Precision>(res);
}

DtLine::Precision DtLine::minMode()
{
    return seconds1;
}

DtLine::Precision DtLine::maxMode()
{
    return years1;
}

QString DtLine::modeToString(DtLine::Precision mode)
{
    switch(mode)
    {
    case seconds1:
        return "seconds1";
    case seconds5:
        return "seconds5";
    case seconds15:
        return "seconds15";
    case seconds30:
        return "seconds30";
    case minutes1:
        return "minutes1";
    case minutes5:
        return "minutes5";
    case minutes15:
        return "minutes15";
    case minutes30:
        return "minutes30";
    case hours1:
        return "hours1";
    case hours6:
        return "hours6";
    case hours12:
        return "hours12";
    case days1:
        return "days1";
    case months1:
        return "months1";
    case years1:
        return "years1";
    default:
        return "default";
    }
}

QString DtLine::modeToString(DtLine::TopPrecision mode)
{
    switch(mode)
    {
    case topYear:
        return "topYear";
    case topMonth:
        return "topMonth";
    case topDay:
        return "topDay";
    case topHour:
        return "topHour";
    case topMinute:
        return "topMinute";
    default:
        qWarning("modeToString <TopPrecision> out of range");
        return "default";
    }
}

DtLine::DtLine(QWidget *parent) : QWidget(parent)
{
    init();
}
void DtLine::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    drawBackground(&painter);
    drawBottom(&painter);
    drawTop(&painter);

}

void DtLine::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    recalc();
}

void DtLine::init()
{
    setMaximumHeight(heightConstraint);
    setMinimumHeight(heightConstraint);

    connect(this,SIGNAL(minChanged()),this,SIGNAL(rangeChanged()));
    connect(this,SIGNAL(timeSpanChanged()),this,SIGNAL(rangeChanged()));
    connect(this,SIGNAL(rangeChanged()),this,SLOT(recalc()));

    connect(this,SIGNAL(minChangedManually()),this,SLOT(emitChangedManually()));
    connect(this,SIGNAL(timeSpanChangedManually()),this,SLOT(emitChangedManually()));

    connect(this,SIGNAL(minChangedManually()),this,SIGNAL(minChanged()));
    connect(this,SIGNAL(timeSpanChangedManually()),this,SIGNAL(timeSpanChanged()));
    connect(this,SIGNAL(rangeChangedManually(UtcDateTime,TimeSpan)),this,SIGNAL(rangeChanged()));
}

void DtLine::drawBackground(QPainter *painter)
{
    // Header background
    QLinearGradient linearGradient(
                QPointF(rect().left(),rect().top()),
                QPointF(rect().left(),rect().top() + heightConstraint));
    linearGradient.setColorAt(0, SLIDER_COLOR);
    linearGradient.setColorAt(0.4, Qt::white);
    linearGradient.setColorAt(0.6, Qt::white);
    linearGradient.setColorAt(1, SLIDER_COLOR);
    painter->fillRect(rect(), linearGradient);
    // Center horizontal line
    painter->drawLine(QPointF(0,heightConstraint/2),
                     QPointF(rect().right(),heightConstraint/2) );
    // Little and big hatches(with text)
}

void DtLine::drawBottom(QPainter *painter)
{
    if(timeSpanIsValid(_timeSpan))
    {
        UtcDateTime dt;
        UtcDateTime nextDt = displayedDtFewer(_min, _mode);
        int l,r;


        do{
            dt = nextDt;
            nextDt = displayedDtNextHatch(dt, _mode);
            l = dtToPos(dt);
            r = dtToPos(nextDt);
            if(isDrawn(dt,_mode)){
                if(extMode(_mode)){
                    painter->drawLine(QPointF(l,itemHeight),
                                      QPointF(l,itemHeight*2)); // very big hatch
                }
                else
                    painter->drawLine(QPointF(l,itemHeight),
                                      QPointF(l,itemHeight*(3.0/2))); // big hatch

                drawBottomItemText(painter, dt, _mode);
            }
            else{
                painter->drawLine(QPointF(l,itemHeight),
                                  QPointF(l,itemHeight*(5.0/4))); // little hatch
            }
        } while(r <= rect().right());
    }
}

void DtLine::drawBottomItemText(QPainter *painter, const UtcDateTime &dt, DtLine::Precision mode)
{
    static QFont dtFont("Goudy Old Style", 8);
    int curPos = dtToPos(dt);
    int nextPos = dtToPos(displayedDtNext(dt,mode));
    const QString text(dt.toString(formatForMode(mode)));
    painter->save();
    painter->setFont(dtFont);
    if(extMode(mode)){
        drawBottomItemTextExt(painter,text,curPos,nextPos,mode);
    }
    else {
        painter->drawText(QRectF(-MIN_WIDTH_FOR_TIME_VISUALIZING/2 + curPos, itemHeight*(3.0/2),
                                 MIN_WIDTH_FOR_TIME_VISUALIZING,itemHeight/2)
                          , text, QTextOption(Qt::AlignCenter));
    }
    painter->restore();
}

void DtLine::drawBottomItemTextExt(QPainter *painter, const QString &text, int curPos, int nextPos, DtLine::Precision mode)
{
    QPair<QRect,Qt::Alignment> textArea = getTextArea(rect(),curPos,nextPos,mode
                                                      , itemHeight * 5. / 4, itemHeight * 3. / 4);

    painter->drawText(textArea.first
                      , text
                      , QTextOption(textArea.second));
}

void DtLine::drawTop(QPainter *painter)
{
    TopPrecision mode = mapToTop(_mode);
    if(mode == topEmpty)
        return; // no paint

    Precision bMode = mapToBottom(mode);

    UtcDateTime dt;
    UtcDateTime nextDt = displayedDtFewer(min(), bMode);
    int l,r;


    do{
        dt = nextDt;
        nextDt = displayedDtNext(dt, bMode);
        l = dtToPos(dt);
        r = dtToPos(nextDt);

        drawTopItem(painter, dt, mode, l, r);
    } while(r <= rect().right());

}

void DtLine::drawTopItem(QPainter *painter, const UtcDateTime &dt, TopPrecision mode, int l, int r)
{
    painter->drawLine(QPointF(l,0),QPointF(l,itemHeight));

    QPair<QRect,Qt::Alignment> textArea = getTextArea(rect(),l,r,mode, 0, itemHeight);

    drawTopItemText(painter, dt.toString(formatForMode(mode))
                    , textArea.first, textArea.second);
}

void DtLine::drawTopItemText(QPainter *painter, const QString &text, const QRect &rect,  Qt::Alignment f)
{
    painter->drawText(rect, text  , QTextOption(f));
}
template<class TPrecision>
QPair<QRect, Qt::Alignment> DtLine::getTextArea(const QRect &rect, int l, int r, TPrecision mode
                                                , int topOffset, int height) const
{
    QRect textRect(l, rect.top() + topOffset , r - l, height);
    Qt::Alignment alignment(Qt::AlignCenter);

    if(l < rect.left()){
        if( r > rect.right()){
            textRect.setLeft(rect.left());
            textRect.setWidth(width());
        }
        else{
            if(r - rect.left() > widthForMode(mode)){
                textRect.setLeft(rect.left());
            }
            else
                alignment = Qt::AlignRight | Qt::AlignVCenter;
        }
    } else if ( r > rect.right()){

        if(rect.right() - l > widthForMode(mode))
            textRect.setWidth(rect.right() - l);
        else
            alignment = Qt::AlignLeft | Qt::AlignVCenter;

    }

    if(alignment != Qt::AlignCenter)
        textRect.adjust(5,0,-5,0);

    return qMakePair(textRect,alignment);
}

TimeSpan DtLine::timeSpan() const
{
    return _timeSpan;
}

bool DtLine::inRange(const UtcDateTime &dt) const
{
    return dt > min() && dt < max();
}

bool DtLine::inRange(int pos) const
{
    return pos >= rect().left() && pos <= rect().right();
}

bool DtLine::inView(const QRect &rect) const
{
    return inRange(rect.left()) || inRange(rect.right());
}

UtcDateTime DtLine::posToDt(int pos) const
{
    // inrange?
    int w = width();
    if(w > 0)
        return _min + _timeSpan * (pos * 1. / w);
    // width == 0
    return UtcDateTime();
}

int DtLine::dtToPos(const UtcDateTime &dt) const
{
    // inrange?
    long long totalMcsecs = _timeSpan.totalMicroseconds();
    if(totalMcsecs != 0)
        return width() * (((double)_min.microsecondsTo(dt)) / totalMcsecs) ;
    return 0;
}

long long DtLine::mcsecsForMode(DtLine::Precision mode, const QDate &date)
{
    switch(mode)
    {
//    case microseconds1:
//        return 1;
//    case microseconds15:
//        return 15;
//    case microseconds30:
//        return 30;
//    case miliseconds1:
//        return _MICROSECONDS_IN_MILISECOND;
//    case miliseconds15:
//        return 15*_MICROSECONDS_IN_MILISECOND;
//    case miliseconds30:
//        return 30*_MICROSECONDS_IN_MILISECOND;
    case seconds1:
        return _MICROSECONDS_IN_SECOND;
    case seconds5:
        return 5*_MICROSECONDS_IN_SECOND;
    case seconds15:
        return 15*_MICROSECONDS_IN_SECOND;
    case seconds30:
        return 30*_MICROSECONDS_IN_SECOND;
    case minutes1:
        return SECONDS_IN_MINUTE*_MICROSECONDS_IN_SECOND;
    case minutes5:
        return 5*SECONDS_IN_MINUTE*_MICROSECONDS_IN_SECOND;
    case minutes15:
        return 15*SECONDS_IN_MINUTE*_MICROSECONDS_IN_SECOND;
    case minutes30:
        return 30*SECONDS_IN_MINUTE*_MICROSECONDS_IN_SECOND;
    case hours1:
        return ((long long)SECONDS_IN_HOUR)*_MICROSECONDS_IN_SECOND;
    case hours6:
        return 6*((long long)SECONDS_IN_HOUR)*_MICROSECONDS_IN_SECOND;
    case hours12:
        return 12*((long long)SECONDS_IN_HOUR)*_MICROSECONDS_IN_SECOND;
    case days1:
        return ((long long)SECONDS_IN_DAY)*_MICROSECONDS_IN_SECOND;
    case months1:
        if(!date.isValid())
            return 28*((long long)SECONDS_IN_DAY)*_MICROSECONDS_IN_SECOND;
        return date.daysInMonth()*((long long)SECONDS_IN_DAY)*_MICROSECONDS_IN_SECOND;
    case years1:
        if(!date.isValid())
            return 345*((long long)SECONDS_IN_DAY)*_MICROSECONDS_IN_SECOND;
        return date.daysInYear()*((long long)SECONDS_IN_DAY)*_MICROSECONDS_IN_SECOND;
    default:
        qWarning("mcsecsForMode out of range");
        return 0;
    }
}

long long DtLine::secsForMode(DtLine::Precision mode, const QDate &date)
{
    switch(mode)
    {
    case seconds1:
        return 1;
    case seconds5:
        return 5;
    case seconds15:
        return 15;
    case seconds30:
        return 30;
    case minutes1:
        return SECONDS_IN_MINUTE;
    case minutes5:
        return 5*SECONDS_IN_MINUTE;
    case minutes15:
        return 15*SECONDS_IN_MINUTE;
    case minutes30:
        return 30*SECONDS_IN_MINUTE;
    case hours1:
        return ((long long)SECONDS_IN_HOUR);
    case hours6:
        return 6*((long long)SECONDS_IN_HOUR);
    case hours12:
        return 12*((long long)SECONDS_IN_HOUR);
    case days1:
        return ((long long)SECONDS_IN_DAY);
    case months1:
        if(!date.isValid())
            return 28*((long long)SECONDS_IN_DAY);
        return date.daysInMonth()*((long long)SECONDS_IN_DAY);
    case years1:
        if(!date.isValid())
            return 345*((long long)SECONDS_IN_DAY);
        return date.daysInYear()*((long long)SECONDS_IN_DAY);
    default:
        qWarning("secsForMode out of range");
        return 0;
    }
}

int DtLine::segmentCountForMode(DtLine::Precision mode, const QDate &date)
{
    switch(mode)
    {
    case seconds1:
        return 1;
    case seconds5:
        return 5;
    case seconds15:
        return 3;
    case seconds30:
        return 6;
    case minutes1:
        return 6;
    case minutes5:
        return 5;
    case minutes15:
        return 3;
    case minutes30:
        return 6;
    case hours1:
        return 6;
    case hours6:
        return 6;
    case hours12:
        return 6;
    case days1:
        return 6;
    case months1:
        if(!date.isValid())
            return 6;
        return date.daysInMonth();
    case years1:
        return 12;
    default:
        qWarning("segmentCountForMode out of range");
        return 1;
    }
}

QString DtLine::formatForMode(DtLine::Precision mode)
{
    switch(mode)
    {
    case seconds1:
    case seconds5:
    case seconds15:
    case seconds30:
        return "mm:ss";
    case minutes1:
    case minutes5:
    case minutes15:
    case minutes30:
        return "hh:mm";
    case hours1:
    case hours6:
    case hours12:
        return "hh:00";

    case days1:
        return "dd";
    case months1:
        return "MMM";
    case years1:
        return "yyyy";
    default:
        qWarning("formatForMode out of range");
        return QString();
    }
}

QString DtLine::formatForMode(DtLine::TopPrecision mode)
{
    switch(mode)
    {
    case topYear:
        return "yyyy";
    case topMonth:
        return "MMM yyyy";
    case topDay:
        return "dd MMM yyyy";
    case topHour:
        return "hh:00 dd MMM yyyy";
    case topMinute:
        return "hh:mm dd MMM yyyy";
    default:
        qWarning("formatForMode out of range");
        return QString();
    }
}

int DtLine::widthForMode(DtLine::TopPrecision mode)
{
    switch(mode)
    {
    case topYear:
        return 35;
    case topMonth:
        return 65;
    case topDay:
        return 100;
    case topHour:
    case topMinute:
        return 130;
    default:
        qWarning("formatForMode out of range");
        return 1;
    }
}

int DtLine::widthForMode(DtLine::Precision mode)
{
    return (mode==months1
                ?MIN_WIDTH_FOR_MONTH_VISUALIZING
                :MIN_WIDTH_FOR_TIME_VISUALIZING);
}

bool DtLine::isDrawn(const UtcDateTime &dt, DtLine::Precision mode)
{
    if(!dt.isValid())
    {
        qWarning("isDrawn dt is not valid");
        return false;
    }

    switch(mode)
    {
    case seconds1:
        return true;
    case seconds5:
        return !(dt.second()%5);
    case seconds15:
        return !(dt.second()%15);
    case seconds30:
        return !(dt.second()%30);
    case minutes1:
        return !(dt.second());
    case minutes5:
        return !(dt.minute()%5);
    case minutes15:
        return !(dt.minute()%15);
    case minutes30:
        return !(dt.minute()%30);
    case hours1:
        return !(dt.minute());
    case hours6:
        return !(dt.hour()%6);
    case hours12:
        return !(dt.hour()%12);
    case days1:
        return !(dt.hour());
    case months1:
        return !(dt.day() - 1);
    case years1:
        return !(-dt.daysTo(UtcDateTime(QDate(dt.year(),1,1))));
    default:
        qWarning("isDrawn out of range");
        return false;
    }
}

UtcDateTime DtLine::displayedDtFewer(const UtcDateTime &dt, DtLine::Precision mode)
{
    switch(mode)
    {
    case seconds1:
        return dt.dateTime();
    case seconds5:
    {
        int secBeg = dt.second()/5 * 5;
        return UtcDateTime(dt.date(),QTime(dt.hour(),dt.minute(),secBeg));
    }
    case seconds15:
    {
        int secBeg = dt.second()/15 * 15;
        return UtcDateTime(dt.date(),QTime(dt.hour(),dt.minute(),secBeg));
    }
    case seconds30:
    {
        int secBeg = dt.second()/30 * 30;
        return UtcDateTime(dt.date(),QTime(dt.hour(),dt.minute(),secBeg));
    }
    case minutes1:
        return UtcDateTime(QDate(dt.year(),dt.month(),dt.day()),QTime(dt.hour(),dt.minute()));
    case minutes5:
    {
        int minBeg = dt.minute()/5 * 5;
        return UtcDateTime(dt.date(),QTime(dt.hour(),minBeg));
    }
    case minutes15:
    {
        int minBeg = dt.minute()/15 * 15;
        return UtcDateTime(dt.date(),QTime(dt.hour(),minBeg));
    }
    case minutes30:
    {
        int minBeg = dt.minute()/30 * 30;
        return UtcDateTime(dt.date(),QTime(dt.hour(),minBeg));
    }
    case hours1:
        return UtcDateTime(QDate(dt.year(),dt.month(),dt.day()),QTime(dt.hour(),0));
    case hours6:
    {
        int hourBeg = dt.hour() / 6 * 6;
        return UtcDateTime(QDate(dt.year(),dt.month(),dt.day()),QTime(hourBeg,0));
    }
    case hours12:
    {
        int hourBeg = dt.hour() / 12 * 12;
        return UtcDateTime(QDate(dt.year(),dt.month(),dt.day()),QTime(hourBeg,0));
    }
    case days1:
        return UtcDateTime(dt.date());
    case months1:
        return UtcDateTime(QDate(dt.year(),dt.month(),1));
    case years1:
        return UtcDateTime(QDate(dt.year(),1,1));
    default:
        qWarning("displayedDtFewer out of range");
        return UtcDateTime();
    }
}

UtcDateTime DtLine::displayedDtGreater(const UtcDateTime &dt, DtLine::Precision mode)
{
    switch(mode)
    {
    case seconds1:
        return UtcDateTime(
                    dt.dateTime()
                    );
//            if(minEnd < dt.minute())
//                m_finishDt.addSecs(1);
    case seconds5:
    {
        int secEnd = dt.second()/5 * 5;
        UtcDateTime res = UtcDateTime(
                    dt.date(),QTime(dt.hour(),dt.minute(),secEnd)
                    );
        if(secEnd < dt.second())
            res = res.addSecs(5);
        return res;
    }
    case seconds15:
    {
        int secEnd = dt.second()/15 * 15;
        UtcDateTime res = UtcDateTime(
                    dt.date(),QTime(dt.hour(),dt.minute(),secEnd)
                    );
        if(secEnd < dt.second())
            res = res.addSecs(15);
        return res;
    }
    case seconds30:
    {
        int secEnd = dt.second()/30 * 30;
        UtcDateTime res = UtcDateTime(
                    dt.date(),QTime(dt.hour(),dt.minute(),secEnd)
                    );
        if(secEnd < dt.second())
            res = res.addSecs(30);
        return res;
    }
    case minutes1:
    {
        UtcDateTime res = UtcDateTime(
                    (QDate(dt.year(),dt.month(),dt.day()))
                    ,QTime(dt.hour(),dt.minute())
                    );
        if(res < dt)
            res = res.addSecs(SECONDS_IN_MINUTE);
        return res;
    }
    case minutes5:
    {
        int minEnd = dt.minute()/5 * 5;
        UtcDateTime res = UtcDateTime(
                    dt.date(),QTime(dt.hour(),minEnd)
                    );
        if(minEnd < dt.minute())
            res = res.addSecs(5*SECONDS_IN_MINUTE);
        return res;
    }
    case minutes15:
    {
        int minEnd = dt.minute()/15 * 15;
        UtcDateTime res = UtcDateTime(
                    dt.date(),QTime(dt.hour(),minEnd)
                    );
        if(minEnd < dt.minute())
            res = res.addSecs(15*SECONDS_IN_MINUTE);
        return res;
    }
    case minutes30:
    {
        int minEnd = dt.minute()/30 * 30;
        UtcDateTime res = UtcDateTime(
                    dt.date(),QTime(dt.hour(),minEnd)
                    );
        if(minEnd < dt.minute())
            res = res.addSecs(30*SECONDS_IN_MINUTE);
        return res;
    }
    case hours1:
    {
        UtcDateTime res = UtcDateTime(
                    (QDate(dt.year(),dt.month(),dt.day()))
                    ,QTime(dt.hour(),0)
                    );
        if(res < dt)
            res = res.addSecs(SECONDS_IN_HOUR);
        return res;
    }
    case hours6:
    {
        int hourEnd = dt.hour() / 6 * 6;
        UtcDateTime res = UtcDateTime(
                    (QDate(dt.year(),dt.month(),dt.day()))
                    ,QTime(hourEnd,0)
                    );


        if(hourEnd < dt.hour())
            res = res.addSecs(secsForMode(mode));

        return res;
    }
    case hours12:
    {
        int hourEnd = dt.hour() / 12 * 12;
        UtcDateTime res = UtcDateTime(
                    (QDate(dt.year(),dt.month(),dt.day()))
                    ,QTime(hourEnd,0)
                    );


        if(hourEnd < dt.hour())
            res = res.addSecs(secsForMode(mode));

        return res;
    }
    case days1:
    {
        UtcDateTime res = UtcDateTime(
                    dt.date()
                    );
        if(res < dt)
            res = res.addDays(1);
        return res;
    }
    case months1:
    {
        UtcDateTime res = UtcDateTime(
                    (QDate(dt.year(),dt.month(),1))
                    );
        if(res < dt)
            res = res.addMonths(1);
        return res;
    }
    case years1:
    {
        UtcDateTime res = UtcDateTime(
                    (QDate(dt.year(),1,1))
                    );
        if(res < dt)
            res = res.addYears(1);
        return res;
    }
    default:
        qWarning("displayedDtGreater out of range");
        return UtcDateTime();
    }
}

UtcDateTime DtLine::displayedDtNext(const UtcDateTime &dt, DtLine::Precision mode)
{
    return dt.addMicroseconds(mcsecsForMode(mode,dt.date()));
}

UtcDateTime DtLine::displayedDtNextHatch(const UtcDateTime &dt, DtLine::Precision mode)
{
    return dt.addMicroseconds(mcsecsForMode(greaterPrecision(mode),dt.date()));
}

//UtcDateTime DtLine::displayedDtPrevious(const UtcDateTime &dt, DtLine::Precision mode)
//{
//    return dt.addMicroseconds(-mcsecsForMode(mode));
//}

DtLine::TopPrecision DtLine::mapToTop(Precision bottomMode)
{
    switch(bottomMode)
    {
    case seconds1:
    case seconds5:
    case seconds15:
    case seconds30:
        return topMinute;
    case minutes1:
    case minutes5:
    case minutes15:
    case minutes30:
    case hours1:
    case hours6:
    case hours12:
        return topDay;
    case days1:
        return topMonth;
    case months1:
        return topYear;
    case years1:
        return topEmpty;
    default:
        qCritical("mapToTop out of range");
        return TopPrecision_count;
    }
}

DtLine::Precision DtLine::mapToBottom(DtLine::TopPrecision topMode)
{
    switch(topMode){
    case topYear:
        return years1;
    case topMonth:
        return months1;
    case topDay:
        return days1;
    case topHour:
        return hours1;
    case topMinute:
        return minutes1;
    default:
        qWarning("mapToBottom default");
        return Precision_count;
    }
}

bool DtLine::extMode(DtLine::Precision mode)
{
    switch(mode)
    {
    case seconds1:
    case seconds5:
    case seconds15:
    case seconds30:
    case minutes1:
    case minutes5:
    case minutes15:
    case minutes30:
    case hours1:
    case hours6:
    case hours12:
        return false;
    case days1:
    case months1:
    case years1:
        return true;
    default:
        qCritical("extMode out of range");
        return false;
    }
}

int DtLine::precisiousWidth() const
{
    UtcDateTime dt1 = displayedDtFewer(min() , _mode),dt2 = displayedDtGreater(min() , _mode);
    int appWidth = 0;
    int tmp;
    const int visualItemWidth = (_mode==months1
                              ?MIN_WIDTH_FOR_MONTH_VISUALIZING
                              :MIN_WIDTH_FOR_TIME_VISUALIZING);
    if(inRange((tmp = dtToPos(dt1) + visualItemWidth/2))
       && !inRange(tmp - visualItemWidth) ){
        appWidth += rect().left() - (tmp - visualItemWidth) ;
    } else if (dt1!=dt2
               && !inRange((tmp = dtToPos(dt2) - visualItemWidth/2))
               && inRange(tmp + visualItemWidth)){
        appWidth += rect().left() - tmp;
    }
    dt1 = displayedDtFewer(max() , _mode); dt2 = displayedDtGreater(max() , _mode);
    if(inRange((tmp = dtToPos(dt2) - visualItemWidth/2))
       && inRange(tmp + visualItemWidth) ){
        appWidth += (tmp + visualItemWidth) - rect().right();
    } else if (dt1!=dt2
               && !inRange((tmp = dtToPos(dt1) + visualItemWidth/2))
               && inRange(tmp - visualItemWidth) ){
        appWidth += tmp - rect().right();
    }

    return width() + appWidth;
}

int DtLine::calcVisItemCount(int width,int itemWidth) const
{
    if(!itemWidth)
        return 1;
    return (width-itemWidth)/itemWidth;
}

DtLine::Precision DtLine::calculateTimeMode() const
{
    if(!timeSpanIsValid(_timeSpan) || width() < MIN_WIDTH_FOR_TIME_VISUALIZING)
        return (Precision)0;

    if(_min.year() == 1998){
        formatForMode(_mode);
    }
    qreal   coef = (_timeSpan.totalMicroseconds() * 1. /
                    calcVisItemCount(precisiousWidth(), MIN_WIDTH_FOR_TIME_VISUALIZING)),
            monthCoef = (_timeSpan.totalMicroseconds() * 1. /
                         calcVisItemCount(precisiousWidth(), MIN_WIDTH_FOR_MONTH_VISUALIZING));

    Precision mode = Precision_count;

    for(int i = Precision_count-1; i >= 0; --i)
    {
        Precision i_mode = static_cast<Precision>(i);
        const qreal &usageCoef = (i_mode == months1)?(monthCoef):(coef);

        if(usageCoef <= mcsecsForMode(i_mode))
        {
            mode = i_mode;
            break;
        }
    }

    if(mode == Precision_count)
    {
        qWarning("calculateTimeMode() out of range");

        return (Precision)0;
    }

    return mode;
}

void DtLine::recalc()
{
    _mode = calculateTimeMode();
    update();
}


void DtLine::setTimeSpan(const TimeSpan &timeSpan, bool manually)
{
    if(timeSpan < _minTimeSpan)
        _timeSpan = _minTimeSpan;
    else
        _timeSpan = timeSpan;

    if(manually)
        emit timeSpanChangedManually();
    else
        emit timeSpanChanged();
    qDebug() << "timeSpan changed";
}

void DtLine::setLimits(const UtcDateTime &min, const TimeSpan &ts, bool manually)
{
    setMin(min);
    setTimeSpan(ts);
}

void DtLine::setLimitsWithOffset(const UtcDateTime &min, const TimeSpan &ts, bool manually)
{
    static const qreal coef = 0.1;
    TimeSpan deltaTs = ts * coef;
    setLimits(min - deltaTs, ts + 2 * deltaTs, manually);
}

void DtLine::zoom(int delta, qreal relPos)
{
    static const qreal  zoom_coef = 0.01,
                        pos_zoom_coef = zoom_coef / 120,
                        neg_zoom_coef = pos_zoom_coef / (1 - 2 * zoom_coef);

    const qreal efficientCoeff = ( delta > 0
                                   ? pos_zoom_coef
                                   : neg_zoom_coef),
            tsReduceCoeff = 1 - 2 * delta * efficientCoeff;
    if(tsReduceCoeff < 0){
        qWarning("DtLine::zoom tsReduceCoeff < 0");
        return;
    }

    if(timeSpanIsValid(_timeSpan) && _min.isValid()){
        setLimits(_min + 2 * relPos * delta * efficientCoeff * _timeSpan,
                  _timeSpan * tsReduceCoeff
                  , true); // manually
    }
}

void DtLine::slide(qreal deltaPercent)
{
    if(timeSpanIsValid(_timeSpan) && _min.isValid()){
        setMin(_min + _timeSpan * deltaPercent, true); // manually
    }
}

void DtLine::emitChangedManually()
{
    emit rangeChangedManually(_min,_timeSpan);
}

UtcDateTime DtLine::min() const
{
    return _min;
}

UtcDateTime DtLine::max() const
{
    return _min + _timeSpan;
}

void DtLine::setMin(const UtcDateTime &min, bool manually)
{
    _min = min;
    if(manually)
        emit minChangedManually();
    emit minChanged();
}
