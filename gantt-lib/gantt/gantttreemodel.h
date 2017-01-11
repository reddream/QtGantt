#ifndef GANTTTREEMODEL_H
#define GANTTTREEMODEL_H

#include "ganttinfoleaf.h"
#include "ganttinfonode.h"

#include <QAbstractItemModel>

class GanttWidget;

class GanttTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Field
    {
        titleField,
        startField,
        finishField,
        durationField,

        Fields_count

    };


public:
    GanttTreeModel(GanttInfoNode *root,QObject * parent = 0);
    ~GanttTreeModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole);

    void addItems(const QList<GanttInfoItem*>& items);
    void addItems(GanttInfoItem* item);

    GanttInfoItem *itemForName(const QString& title) const;


    GanttInfoNode *root() const;


signals:
    void itemsAdded(GanttInfoItem*);
    void itemsAdded();
    void needCollapse(GanttInfoNode*);

public slots:
    void clear();

private:
    GanttInfoItem* itemForIndex(const QModelIndex& index) const;
    GanttInfoLeaf* leafForIndex(const QModelIndex& index) const;
    GanttInfoNode* nodeForIndex(const QModelIndex& index) const;

    void initIndexes(GanttInfoItem* item);
    GanttInfoItem *itemForNameHelper(const QString& title,GanttInfoNode* node) const;

private:

    GanttInfoNode *m_root;

    friend class GanttWidget;

};

#endif // GANTTTREEMODEL_H
