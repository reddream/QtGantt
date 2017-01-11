#ifndef GANTTINFOLEAF_H
#define GANTTINFOLEAF_H

#include "ganttinfoitem.h"

#include "utcdatetime.h"

#include <QColor>

class GANTTMODELSHARED_EXPORT GanttInfoLeaf : public GanttInfoItem
{
    Q_OBJECT

public:
    GanttInfoLeaf(QObject *parent = NULL);

    UtcDateTime start() const;
    UtcDateTime finish() const;
    QColor getColor() const;

    long long duration() const; // in microsec

    int columnCount() const;
    qreal height() const;
    void callForEachItemRecursively(void (*func)(GanttInfoItem*));

signals:
    void startChanged(/*UtcDateTime prevStart*/);
    void finishChanged(/*UtcDateTime prevFinish*/);
    void colorChanged();

public slots:
    void setStart(const UtcDateTime &start);
    void setFinish(const UtcDateTime &finish);
    void setColor(const QColor &value);

private:
    UtcDateTime m_start,
            m_finish;
    QColor m_color;

};

#endif // GANTTINFOITEM_H
