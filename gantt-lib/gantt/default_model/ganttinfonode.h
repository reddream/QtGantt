#ifndef GANTTINFONODE_H
#define GANTTINFONODE_H

#include "ganttinfoleaf.h"

#include <QList>
#include <QModelIndex>

class GanttScene;

class GANTTLIBSHARED_EXPORT GanttInfoNode : public GanttInfoItem
{
    Q_OBJECT

    void init();
public:
    GanttInfoNode(QObject *parent = NULL);
    GanttInfoNode(const QString &title
                  , const UtcDateTime   &start
                  , const TimeSpan      &ts
                  , const QModelIndex   &index
                  , const QColor        &color = Qt::green
                  , GanttInfoNode       *parentNode = NULL
                  , QObject             *parent = NULL);

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



    bool isExpanded() const;
    void setExpanded(bool expanded);
    void changeExpanding();

    qreal height() const;
    int columnCount() const;
    GanttInfoNode *node();
    void callForEachItemRecursively(void (*func)(GanttInfoItem*));

public slots:
    void updatePos();
    
signals:
    void itemsChanged();
    void calcDtChanged();
    
protected:
    int indexOf(const GanttInfoItem * p_item) const;

protected slots:
    void onItemExpandingChange(int id);
private slots:
    void onSelfExpandingChange();

private:

    QList<GanttInfoItem*> _items;
    
    bool _expanded;

    friend class GanttInfoItem;
};

#endif // GANTTINFO_H
