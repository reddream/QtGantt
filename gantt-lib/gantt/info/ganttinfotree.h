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
    void init();
public:
    explicit GanttInfoTree(QObject *parent = 0);

    QAbstractItemModel *model() const;
    void setModel(IGanttModel *model);
    void connectTreeView(QTreeView *view);
    void disconnectTreeView(QTreeView *view);
    GanttInfoItem *infoForIndex(const QModelIndex &index, GanttInfoItem *item = NULL) const;
    GanttInfoNode *root() const;
    GanttInfoItem *infoForVPos(int vpos);
    int height() const;


signals:
    void treeReset();
    void limitsChanged(const UtcDateTime &min, const TimeSpan &ts);
    void rowsInserted(GanttInfoNode *parent, int from, int to);
    void endRemoveItems();
    void itemAdded(GanttInfoItem *item);
    void itemAboutToBeDeleted(GanttInfoItem *item);
    void itemRemoved(GanttInfoItem *item);
    void currentChanged(const QModelIndex &index);
    void currentChanged(GanttInfoItem *item);
    void needExpand(const QModelIndex &index);
    void needCollapse(const QModelIndex &index);
    void expanded(GanttInfoNode *which);
    void collapsed(GanttInfoNode *which);

public slots:
    void onCurrentItemChanged(GanttInfoItem *item);
    void onNodeExpanded();
    void onNodeCollapsed();
    void onClicked(const QModelIndex &index);
    void onExpanded(const QModelIndex &index);
    void onCollapsed(const QModelIndex &index);

    void clear();
    void reset();
private slots:
    void onDataChanged(const QModelIndex &from, const QModelIndex &to);
//    void onBeginInsertRows(const QModelIndex &parent, int from, int to);
    void onRowsInserted(const QModelIndex &parent, int start, int end);
    void onColumnsInserted(const QModelIndex &parent, int start, int end);
    void onRowsRemoved(const QModelIndex &parent, int start, int end);
    void onColumnsRemoved(const QModelIndex &parent, int start, int end);
    void onItemAboutToBeDeleted();

    void connectNewItem(GanttInfoItem *item);
    void updateLimits();
    void updateLimitsByItem(GanttInfoItem *item);
    void emitLimitsChanged();
    void collapseAll();

private:
    void setLimits(const QPair<UtcDateTime, UtcDateTime> &newLimits);
    int heightH(GanttInfoItem *item) const;
    void onAnyAddition();
    GanttInfoItem *lookupForVPos(int vpos, GanttInfoNode *node);
    void fillRecursive(GanttInfoItem *item,const QModelIndex &index);
    void fill(GanttInfoNode *node, const QModelIndex &index, int from, int to);
    GanttInfoItem *makeInfoItem(const QModelIndex &index);
    void disconnectLastModel();
    void connectNewModel();

    QAbstractItemModel *_model; ///< Ссылаются на одно и то же
    IGanttModel *_iGanttModel;  ///< Ссылаются на одно и то же
    GanttInfoNode *_root;

    QPair<UtcDateTime, UtcDateTime> _limits;    // caches Limits
    bool _limitsChanged;

};

#endif // GANTTINFOTREE_H
