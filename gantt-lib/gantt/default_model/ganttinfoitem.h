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
                  , const UtcDateTime   &finish
                  , const QModelIndex   &index
                  , const QColor   &color = Qt::green
                  , GanttInfoNode       *parentNode = NULL
                  , QObject             *parent = NULL);
    virtual ~GanttInfoItem();

    GanttInfoNode *parent() const;
    int row() const;

    virtual int columnCount() const = 0;
    virtual qreal height() const = 0;
    virtual qreal pos() const;

    QModelIndex index() const;

    QString     title() const;
    UtcDateTime start() const;
    UtcDateTime finish() const;
    QColor      color() const;

    bool hasStart() const;
    bool hasFinish() const;
    long long   duration() const;



    static QPair<UtcDateTime,UtcDateTime> getLimits(const GanttInfoItem *root);
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
    void finishChanged();
    void colorChanged();

    void changed();

public slots:
    void setStart(const UtcDateTime &start);
    void setFinish(const UtcDateTime &finish);
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
    UtcDateTime _start, _finish;
    QColor _color;

    unsigned int _linkCnt;
    bool _deleted;
    QMutex _mutex;
};

#endif // GANTTINFOITEM_H
