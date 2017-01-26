#ifndef GANTTINFOITEM_H
#define GANTTINFOITEM_H

#include "gantt-lib_global.h"
#include "utcdatetime.h"

#include <QColor>
#include <QString>
#include <QModelIndex>
#include <QMutex>

class GanttInfoNode;
class UtcDateTime;

class GANTTLIBSHARED_EXPORT GanttInfoItem : public QObject
{
    Q_OBJECT

public:
    GanttInfoItem(QObject *parent = NULL);
    GanttInfoItem(const QString &title
                  , const UtcDateTime   &start
                  , const TimeSpan      &ts
                  , const QModelIndex   &index
                  , const QColor        &color = Qt::green
                  , GanttInfoNode       *parentNode = NULL
                  , QObject             *parent = NULL);
    virtual ~GanttInfoItem();

    GanttInfoNode *parent() const;
    int row() const;

    virtual int columnCount() const = 0;
    virtual qreal height() const = 0;
    virtual qreal calcPos() const;
    int pos() const;
    void setPos(int pos);

    QModelIndex index() const;

    QString     title() const;
    UtcDateTime start() const;
    TimeSpan timeSpan() const;
    UtcDateTime finish() const;
    QColor      color() const;

    bool hasStart() const;
    bool isDot() const;
    virtual GanttInfoNode *node() = 0;



    static QPair<UtcDateTime,UtcDateTime> getLimits(const GanttInfoItem *root);
    QPair<UtcDateTime,UtcDateTime> getLimits() const;
    virtual bool operator==(const GanttInfoItem& another);
    virtual void callForEachItemRecursively(void (*func)(GanttInfoItem*)) = 0;

    void increaseLinkCnt();
    void reduceLinkCnt();
    virtual int indexOf(const GanttInfoItem* p_item) const;
signals:
    void expanded();
    void collapsed();
    void parentChanged();
    void indexChanged();
    void titleChanged();
    void aboutToBeDeleted();
    void startChanged();
    void timeSpanChanged();
    void colorChanged();
    void posChanged();

    void changed();

public slots:
    virtual void updatePos();
    void setStart(const UtcDateTime &start);
    void setTimeSpan(const TimeSpan &ts);
    void setColor(const QColor &color);
    void setTitle(const QString &title);
    void setIndex(const QModelIndex &index);
    void setParent(GanttInfoNode *parent);
    void deleteInfoItem();
    void tryDelete();

protected:

private:
    void init();

    GanttInfoNode* _parent;
    QModelIndex _index;

    QString _title;
    UtcDateTime _start;
    TimeSpan _timeSpan;
    QColor _color;

    unsigned int _linkCnt;
    bool _deleted;
    QMutex _mutex;
    int _pos;
};

#endif // GANTTINFOITEM_H
