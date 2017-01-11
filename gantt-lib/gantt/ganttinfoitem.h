#ifndef GANTTINFOITEM_H
#define GANTTINFOITEM_H

#include "ganttproject_global.h"

#include <QString>
#include <QModelIndex>
#include <QMutex>

class GanttInfoNode;
class UtcDateTime;

class GANTTMODELSHARED_EXPORT GanttInfoItem : public QObject
{
    Q_OBJECT

public:
    GanttInfoItem(QObject *parent = NULL);
    virtual ~GanttInfoItem();

    GanttInfoNode *parent() const;

    virtual int columnCount() const = 0;

    QString title() const;

    int row() const;

    virtual qreal height() const = 0;
    virtual qreal pos() const;

    QModelIndex index() const;
    virtual void callForEachItemRecursively(void (*func)(GanttInfoItem*)) = 0;

    void increaseLinkCnt();
    void reduceLinkCnt();

    static QPair<UtcDateTime,UtcDateTime> getLimits(const GanttInfoItem *root);

signals:
    void parentChanged();
    void indexChanged();
    void titleChanged();
    void aboutToBeDeleted();

    void changed();

public slots:
    void setTitle(const QString &title);
    void setIndex(const QModelIndex &index);
    void setParent(GanttInfoNode *parent);
    void deleteInfoItem();
    void tryDeleteHimself();

protected:
    virtual int indexOf(const GanttInfoItem* p_item) const;

private:
    GanttInfoNode* m_parent;
    QModelIndex m_index;
    QString m_title;

    unsigned int m_linkCnt;
    bool m_deleted;
    QMutex m_mutex;
};

#endif // GANTTINFOITEM_H
