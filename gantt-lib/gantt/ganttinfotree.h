#ifndef GANTTINFOTREE_H
#define GANTTINFOTREE_H

#include "ganttinfonode.h"
#include "iganttmodel.h"

#include <QObject>
#include <QAbstractItemModel>
#include <QTreeView>

class GanttInfoTree : public QObject
{
    Q_OBJECT
public:
    explicit GanttInfoTree(QObject *parent = 0);

    void setModel(IGanttModel *model);
    void connectTreeView(QTreeView *view);
    void disconnectTreeView(QTreeView *view);
    GanttInfoItem *itemForIndex(const QModelIndex &index, GanttInfoItem *item = NULL) const;
    GanttInfoItem *root() const;

signals:
    void endInsertItems();
    void endRemoveItems();
    void itemAdded(GanttInfoItem *item);
    void itemRemoved(GanttInfoItem *item);
    void currentChanged(const QModelIndex &index);
    void needExpand(const QModelIndex &index);
    void needCollapse(const QModelIndex &index);

public slots:
    void onClicked(const QModelIndex &index);
    void onExpanded(const QModelIndex &index);
    void onCollapsed(const QModelIndex &index);

    void clear();
    void reset();
    void onDataChanged(const QModelIndex &from, const QModelIndex &to);


private:
    void fillRecursive(GanttInfoItem *item,const QModelIndex &index);
    GanttInfoItem *makeInfoItem(const QModelIndex &index);
    void init();
    void disconnectLastModel();
    void connectNewModel();

    QAbstractItemModel *_model; ///< Ссылаются на одно и то же
    IGanttModel *_iGanttModel;  ///< Ссылаются на одно и то же

    GanttInfoNode *_root;

};

#endif // GANTTINFOTREE_H
