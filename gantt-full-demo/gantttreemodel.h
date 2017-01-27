#ifndef GANTTTREEMODEL_H
#define GANTTTREEMODEL_H

#include "gantt-lib_global.h"
#include "ganttinfoleaf.h"
#include "ganttinfonode.h"
#include "iganttmodel.h"

#include <QAbstractItemModel>

class GanttWidget;

class GanttTreeModel : public QAbstractItemModel, public IGanttModel
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
    GanttTreeModel(GanttInfoNode *root = NULL,QObject * parent = 0);
    ~GanttTreeModel();

    QVariant data(const QModelIndex &iGanttIndex, int role) const;
    Qt::ItemFlags flags(const QModelIndex &iGanttIndex) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &iGanttIndex) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    bool setData(const QModelIndex &iGanttIndex, const QVariant &value,
                 int role = Qt::EditRole);

    void addItems(const QList<GanttInfoItem*>& items);
    void addNode(GanttInfoNode *node);
//    void addItems(GanttInfoItem* item, bool inner = false);
    void insertItem(GanttInfoItem *item, GanttInfoNode *parent = NULL);

    GanttInfoItem *itemForName(const QString& iGanttTitle) const;


    GanttInfoNode *root() const;

    // --- Interface implementation

    QString     iGanttTitle(const QModelIndex &index) const {
        GanttInfoItem *item =itemForIndex(index);
        if(item)
            return item->title();
        return QString();
    }

    UtcDateTime iGanttStart(const QModelIndex &index) const {
        GanttInfoItem *item =itemForIndex(index);
        if(item)
            return item->start();
        return UtcDateTime();
    }
    TimeSpan iGanttTimeSpan(const QModelIndex &index) const {
        GanttInfoItem *item =itemForIndex(index);
        if(item)
            return item->timeSpan();
        return TimeSpan();
    }
    QColor      iGanttColor(const QModelIndex &index) const {
        GanttInfoItem *item =itemForIndex(index);
        if(item)
            return item->color();
        return Qt::green;
    }

    QModelIndex iGanttIndex(const QString &title) const {
        QModelIndexList matches = match(index(0,0),Qt::DisplayRole,QVariant::fromValue(title));
        if(matches.isEmpty()){
            qDebug() << "not found";
            return QModelIndex();
        }
        return matches.at(0);
    }


    // ---


signals:
    void itemsAdded(GanttInfoItem*);
    void itemsAdded();
    void needCollapse(GanttInfoNode*);

public slots:
    void clear();

private:
    GanttInfoItem* itemForIndex(const QModelIndex& iGanttIndex) const;
    GanttInfoLeaf* leafForIndex(const QModelIndex& iGanttIndex) const;
    GanttInfoNode* nodeForIndex(const QModelIndex& iGanttIndex) const;

    void setIndex(GanttInfoItem* item);
    GanttInfoItem *itemForNameHelper(const QString& iGanttTitle,GanttInfoNode* node) const;

private:

    GanttInfoNode *m_root;

    friend class GanttWidget;

};

#endif // GANTTTREEMODEL_H
