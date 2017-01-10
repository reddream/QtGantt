#ifndef DTLINE_H
#define DTLINE_H

#include "gantt-lib_global.h"
#include "utcdatetime.h"
#include "timespan_extension.h"

#include <QWidget>

class GANTTLIBSHARED_EXPORT DtLine : public QWidget
{
    Q_OBJECT

    enum Precision
    {
        years1,
        months1,
        days1,

        hours12,
        hours6,
        hours1,

        minutes30,
        minutes15,
        minutes5,
        minutes1,

        seconds30,
        seconds15,
        seconds5,
        seconds1,

//        miliseconds30,
//        miliseconds15,
//        miliseconds1,

//        microseconds30,
//        microseconds15,
//        microseconds1,

        Precision_count
    };

public:
    explicit DtLine(QWidget *parent = 0);

    UtcDateTime min() const;
    UtcDateTime max() const;
    TimeSpan timeSpan() const;


    bool inRange(const UtcDateTime &dt) const;
    UtcDateTime posToDt(int pos) const;
    int dtToPos(const UtcDateTime &dt) const;


    static long long mcsecsForMode(Precision mode, const QDate& date = QDate());
    static long long secsForMode(Precision mode, const QDate& date = QDate());
    static int segmentCountForMode(Precision mode, const QDate& date = QDate());
    static QString formatForMode(Precision mode);
    static bool isDrawn(const UtcDateTime& dt, Precision mode);
    static UtcDateTime displayedDtFewer(const UtcDateTime &dt, Precision mode);
    static UtcDateTime displayedDtGreater(const UtcDateTime &dt, Precision mode);
    static UtcDateTime displayedDtNext(const UtcDateTime &dt, Precision mode);
    static UtcDateTime displayedDtPrevious(const UtcDateTime &dt, Precision mode);
    int calcVisItemCount(int itemWidth) const;
    Precision calculateTimeMode() const;


public slots:
    void recalc();
    void setMin(const UtcDateTime &min);
    void setTimeSpan(const TimeSpan &timeSpan);

signals:
    void minChanged();
    void timeSpanChanged();
    void changed();

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

private:
    void init();

private:
    Precision m_mode;
    UtcDateTime m_min;
    TimeSpan m_timeSpan;
};

#endif // DTLINE_H
