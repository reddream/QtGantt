#ifndef GANTTINFONODE_H
#define GANTTINFONODE_H

#include "ganttinfoleaf.h"

#include <QList>
#include <QModelIndex>

class GanttScene;

class GANTTLIBSHARED_EXPORT GanttInfoNode : public GanttInfoItem
{
    Q_OBJECT

public:
    GanttInfoNode(QObject *parent = NULL);
    GanttInfoNode(const QString &title
                  , const UtcDateTime &start
                  , const UtcDateTime &finish
                  , const QModelIndex &index
                  , const QColor &color = Qt::green
                  , GanttInfoNode *parentNode = NULL
                  , QObject *parent = NULL);

    GanttInfoLeaf *leafAt(int index) const;
    GanttInfoNode *nodeAt(int index) const;
    GanttInfoItem *at(int index) const;
    GanttInfoItem *operator[](int index) const;

    int size() const;
    bool isEmpty() const;
    void clear();
    void append(GanttInfoItem* item);
    void append(const QList<GanttInfoItem*>& items);
    bool removeOne(GanttInfoItem* item);



    bool expanded() const;
    void setExpanded(bool expanded);

    qreal height() const;
    int columnCount() const;
    void callForEachItemRecursively(void (*func)(GanttInfoItem*));
    
signals:
    void itemsChanged();
    void calcDtChanged();
    
protected:
    int indexOf(const GanttInfoItem * p_item) const;

private:
    void init();

    QList<GanttInfoItem*> _items;
    
    bool _expanded;

    friend class GanttInfoItem;
};

#endif // GANTTINFO_H
