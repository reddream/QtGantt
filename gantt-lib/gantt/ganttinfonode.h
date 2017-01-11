#ifndef GANTTINFONODE_H
#define GANTTINFONODE_H

#include "ganttinfoleaf.h"

#include <QList>
#include <QModelIndex>

class GanttHeader;
class GanttScene;

class GANTTMODELSHARED_EXPORT GanttInfoNode : public GanttInfoItem
{
    Q_OBJECT

public:
    GanttInfoNode(QObject *parent = NULL);

    GanttInfoLeaf *leafAt(int index) const;
    GanttInfoNode *nodeAt(int index) const;
    GanttInfoItem *child(int index) const;

    int size() const;
    bool isEmpty() const;
    void clear();
    void append(GanttInfoItem* item);
    void append(const QList<GanttInfoItem*>& items);
    bool removeOne(GanttInfoItem* item);

    int columnCount() const;

    qreal height() const;

    bool isExpanded() const;
    void setIsExpanded(bool isExpanded);

    UtcDateTime calcDt() const;
    bool hasCalcDt() const;
    void setCalcDt(const UtcDateTime &calcDt);
    void callForEachItemRecursively(void (*func)(GanttInfoItem*));
    
signals:
    void itemsChanged();
    void calcDtChanged();
    
protected:
    int indexOf(const GanttInfoItem * p_item) const;

private:
    QList<GanttInfoItem*> m_items;
    
    UtcDateTime m_calcDt;
    bool m_isExpanded;

    friend class GanttInfoItem;
    friend class GanttHeader;
    friend class GanttScene;
    friend class GanttWidget;
};

#endif // GANTTINFO_H
