#ifndef DTLINE_H
#define DTLINE_H

#include "gantt-lib_global.h"
#include "utcdatetime.h"
#include "timespan_extension.h"

#include <QWidget>
#include <QPair>

class GANTTLIBSHARED_EXPORT DtLine : public QWidget
{
    Q_OBJECT
    void init();
public:

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

    enum TopPrecision{
        topEmpty,
        topYear     = 1,
        topMonth    = 0x2,
        topDay      = 0x4,
        topHour     = 0x8,
        topMinute   = 0x10,

        TopPrecision_count
    };

    static Precision greaterPrecision(Precision mode);
    static Precision lessPrecision(Precision mode);
    static Precision minMode();
    static Precision maxMode();
    static QString modeToString(Precision mode);
    static QString modeToString(TopPrecision mode);

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
    static QString formatForMode(TopPrecision mode);
    static int widthForMode(TopPrecision mode);
    static int widthForMode(Precision mode);
    static bool isDrawn(const UtcDateTime& dt, Precision mode);
    static UtcDateTime displayedDtFewer(const UtcDateTime &dt, Precision mode);
    static UtcDateTime displayedDtGreater(const UtcDateTime &dt, Precision mode);
    static UtcDateTime displayedDtNext(const UtcDateTime &dt, Precision mode);
    static UtcDateTime displayedDtNextHatch(const UtcDateTime &dt, Precision mode);
//    static UtcDateTime displayedDtPrevious(const UtcDateTime &dt, Precision mode); // impl isn't obvious
    static TopPrecision mapToTop(Precision bottomMode);
    static Precision mapToBottom(TopPrecision topMode);
    static bool extMode(Precision mode);
    int precisiousWidth() const;
    int calcVisItemCount(int width,int itemWidth) const;
    Precision calculateTimeMode() const;


public slots:
    void recalc();
    void setMin(const UtcDateTime &min, bool manually = false);
    void setTimeSpan(const TimeSpan &timeSpan, bool manually = false);
    void setLimits(const UtcDateTime &min, const TimeSpan &ts, bool manually = false); ///< setMin + setTimeSpan
    void setLimitsWithOffset(const UtcDateTime &min, const TimeSpan &ts, bool manually = false);

    void zoom(int delta, qreal relPos = 0.5); // default: center
    void slide(qreal deltaPercent); // slide dt

    void emitChangedManually();

signals:
    void minChanged();
    void minChangedManually();
    void timeSpanChanged();
    void timeSpanChangedManually();
    void rangeChanged();
    void rangeChangedManually(const UtcDateTime &min, const TimeSpan &ts);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

private:
    bool inRange(int pos) const;
    bool inView(const QRect &rect) const;

    void drawBackground(QPainter *painter);
    void drawBottom(QPainter *painter);
    void drawBottomItemText(QPainter *painter, const UtcDateTime &dt, Precision mode);
    void drawBottomItemTextExt(QPainter *painter, const QString &text,
                               int curPos, int nextPos,Precision mode);
    void drawTop(QPainter *painter);
    void drawTopItem(QPainter *painter, const UtcDateTime &dt, TopPrecision mode, int l, int r);
    void drawTopItemText(QPainter *painter, const QString &text, const QRect &rect, Qt::Alignment f);
    template<class TPrecision>
    QPair<QRect,Qt::Alignment> getTextArea(const QRect &rect, int l, int r, TPrecision mode
                                           , int topOffset, int height) const;


private:
    Precision _mode;
    UtcDateTime _min;
    TimeSpan _timeSpan;

    static const TimeSpan _minTimeSpan;
};

#endif // DTLINE_H
