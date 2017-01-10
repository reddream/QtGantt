#include "dtline.h"
#include "gantt-lib_global_values.h"

#include <QPainter>

enum defaults {
    itemHeight = 20,
    heightConstraint = 2 * itemHeight,
};

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
        return "";
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


    connect(this,SIGNAL(minChanged()),this,SIGNAL(changed()));
    connect(this,SIGNAL(timeSpanChanged()),this,SIGNAL(changed()));
    connect(this,SIGNAL(changed()),this,SLOT(recalc()));
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
    if(timeSpanIsValid(m_timeSpan))
    {
        UtcDateTime dt = displayedDtFewer(m_min,m_mode);
        while(dt <= max())
        {
            int curOffset=dtToPos(dt);
            if(isDrawn(dt,m_mode))
            {
                painter->drawLine(QPointF(curOffset,itemHeight),QPointF(curOffset,itemHeight*(3.0/2))); // big hatch
                QFont dtFont("Goudy Old Style", 8);
                painter->setFont(dtFont);
                painter->drawText(QRectF(-MIN_WIDTH_FOR_TIME_VISUALIZING/2 + curOffset, itemHeight*(3.0/2),
                                         MIN_WIDTH_FOR_TIME_VISUALIZING,itemHeight/2)
                                  ,dt.toString(formatForMode(m_mode)),QTextOption(Qt::AlignCenter));
            }
            else
                painter->drawLine(QPointF(curOffset,itemHeight),QPointF(curOffset,itemHeight*(5.0/4))); // little hatch

            dt=dt.addMicroseconds(mcsecsForMode(m_mode,dt.date()) / segmentCountForMode(m_mode,dt.date()));
        }
    }
}

void DtLine::drawTop(QPainter *painter)
{

}

TimeSpan DtLine::timeSpan() const
{
    return m_timeSpan;
}

bool DtLine::inRange(const UtcDateTime &dt) const
{
    return dt > min() && dt < max();
}

UtcDateTime DtLine::posToDt(int pos) const
{
    // inrange?
    int w = width();
    if(w > 0)
        return m_min + m_timeSpan * (pos * 1.0 / w);
    // width == 0
    return UtcDateTime();
}

int DtLine::dtToPos(const UtcDateTime &dt) const
{
    // inrange?
    long long totalMcsecs = m_timeSpan.totalMicroseconds();
    if(totalMcsecs != 0)
        return width() * m_min.microsecondsTo(dt) * 1. / totalMcsecs;
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
        qWarning("long long GanttHeader::modeToMicrosecond(GanttHeader::GanttHeaderMode mode) out of range");
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
        qWarning("GanttHeader::modeToSecond out of range");
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
        qWarning("GanttHeader::modeToSecond out of range");
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
        qWarning("GanttHeader::formatForMode out of range");
        return QString();
    }
}

bool DtLine::isDrawn(const UtcDateTime &dt, DtLine::Precision mode)
{
    if(!dt.isValid())
    {
        qWarning("GanttHeader::isDrawn dt is not valid");
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
        qWarning("GanttHeader::isDrawn out of range");
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
        qWarning("GanttHeader::startByDt() out of range");
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
        qWarning("GanttHeader::finishByDt() out of range");
        return UtcDateTime();
    }
}

UtcDateTime DtLine::displayedDtNext(const UtcDateTime &dt, DtLine::Precision mode)
{
    return dt.addMicroseconds(mcsecsForMode(mode));
}

UtcDateTime DtLine::displayedDtPrevious(const UtcDateTime &dt, DtLine::Precision mode)
{
    return dt.addMicroseconds(-mcsecsForMode(mode));
}

int DtLine::calcVisItemCount(int itemWidth) const
{
    if(!itemWidth)
        return 1;
    return (width()-itemWidth)/itemWidth;
}

DtLine::Precision DtLine::calculateTimeMode() const
{
    if(!timeSpanIsValid(m_timeSpan) || width() < MIN_WIDTH_FOR_TIME_VISUALIZING)
        return (Precision)0;

    qreal   coef = (m_timeSpan.totalMicroseconds() * 1. / calcVisItemCount(MIN_WIDTH_FOR_TIME_VISUALIZING-5)),
            monthCoef = (m_timeSpan.totalMicroseconds() * 1. / calcVisItemCount(MIN_WIDTH_FOR_MONTH_VISUALIZING));

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
        qWarning("GanttHeader::calculateTimeMode() out of range");

        return (Precision)0;
    }

    return mode;
}

void DtLine::recalc()
{
    m_mode = calculateTimeMode();
    qDebug() << "mode " << modeToString(m_mode);
    update();
}


void DtLine::setTimeSpan(const TimeSpan &timeSpan)
{
    m_timeSpan = timeSpan;
    emit timeSpanChanged();
}

UtcDateTime DtLine::min() const
{
    return m_min;
}

UtcDateTime DtLine::max() const
{
    return m_min + m_timeSpan;
}

void DtLine::setMin(const UtcDateTime &min)
{
    m_min = min;
    emit minChanged();
}
