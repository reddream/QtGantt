#include "ganttinfotree.h"
#include "gantt-lib_global_values.h"

GanttInfoTree::GanttInfoTree(QObject *parent) : QObject(parent)
{
    init();
}

void GanttInfoTree::setModel(IGanttModel *model)
{
    if(!model)
        return;
    QAbstractItemModel *itemModel = dynamic_cast<QAbstractItemModel*>(model);
    if(!itemModel){
        qWarning("Warning:: trying to set Model not inherited from QAbstractItemModel");
        return;
    }

    disconnectLastModel();
    _iGanttModel = model;
    _model = itemModel;
    connectNewModel();

    reset();
}

void GanttInfoTree::connectTreeView(QTreeView *view)
{
    if(!view)
        return;

    connect(this,SIGNAL(needCollapse(QModelIndex)),view,SLOT(collapse(QModelIndex)));
    connect(this,SIGNAL(needExpand(QModelIndex)),view,SLOT(expand(QModelIndex)));
    connect(this,SIGNAL(currentChanged(QModelIndex)),view,SLOT(setCurrentIndex(QModelIndex)));

    connect(view,SIGNAL(expanded(QModelIndex)),this,SLOT(onExpanded(QModelIndex)));
    connect(view,SIGNAL(collapsed(QModelIndex)),this,SLOT(onCollapsed(QModelIndex)));
    connect(view,SIGNAL(clicked(QModelIndex)),this,SLOT(onClicked(QModelIndex)));

}

void GanttInfoTree::disconnectTreeView(QTreeView *view)
{
    if(view){
        view->disconnect(this);
        disconnect(view);
    }
}

GanttInfoNode *GanttInfoTree::root() const
{
    return _root;
}

GanttInfoItem *GanttInfoTree::infoForVPos(int vpos)
{
    return lookupForVPos(vpos,_root);
}

int GanttInfoTree::height() const
{
    return heightH(_root);
}

void GanttInfoTree::onCurrentItemChanged(GanttInfoItem *item)
{

}

GanttInfoItem *GanttInfoTree::infoForIndex(const QModelIndex &index, GanttInfoItem *item) const
{
    GanttInfoLeaf *leaf = qobject_cast<GanttInfoLeaf*>(item);
    GanttInfoNode *node = qobject_cast<GanttInfoNode*>(item);
    if(!item)
        node = _root;
    if(leaf && leaf->index() == index)
        return leaf;
    if(node){
        if(node->index() == index)
            return node;
        else
            for(int i = 0; i < node->size(); ++i){
                GanttInfoItem *itemForIndexChild = infoForIndex(index, node->at(i));
                if(itemForIndexChild)
                    return itemForIndexChild;
            }
    }
    return NULL;
}

void GanttInfoTree::onClicked(const QModelIndex &index)
{
    emit currentChanged(infoForIndex(index.sibling(index.row(),0)));
}

void GanttInfoTree::onExpanded(const QModelIndex &index)
{
    GanttInfoNode *node = dynamic_cast<GanttInfoNode*>(infoForIndex(index));
    if(node)
        node->setExpanded(true);
}

void GanttInfoTree::onCollapsed(const QModelIndex &index)
{
    GanttInfoNode *node = dynamic_cast<GanttInfoNode*>(infoForIndex(index));
    if(node)
        node->setExpanded(false);
}

void deleteInfoItemFunc(GanttInfoItem* item)
{
    if(item->parent())
    {
        item->deleteInfoItem();
    }
}

void GanttInfoTree::clear()
{
    if(_root)
        _root->callForEachItemRecursively(&deleteInfoItemFunc);
    _root->clear();

    emit endRemoveItems();
}

void GanttInfoTree::reset()
{
    if(!_model){
        qWarning("GanttInfoTree called reset w/o model");
        return;
    }

    clear();
    fillRecursive(_root,QModelIndex());
    qDebug() << "reset, root sz "<<_root->size();

    emit treeReset();
    onAnyAddition();
}

void GanttInfoTree::onNodeExpanded()
{
    GanttInfoNode *node = qobject_cast<GanttInfoNode*>(sender());
    if(!node)
        return;
    emit expanded(node);
    emit needExpand(node->index());
}

void GanttInfoTree::onNodeCollapsed()
{
    GanttInfoNode *node = qobject_cast<GanttInfoNode*>(sender());
    if(!node)
        return;
    emit collapsed(node);
    emit needCollapse(node->index());
}

void GanttInfoTree::onDataChanged(const QModelIndex &/*from*/, const QModelIndex &/*to*/)
{
    /// TODO optimization
    reset();
}

//void GanttInfoTree::onBeginInsertRows(const QModelIndex &parent, int from, int to)
//{
//    qDebug() << "onBeginInsertRows " << parent << ' ' << from << ' ' << to;
//    if(!_model){
//        qWarning("onBeginInsertRows called reset w/o model");
//        return;
//    }
//    for(int i = from; i <= to; ++i){
//        makeInfoItem(parent.child(i,0));
//    }
//}

void GanttInfoTree::onRowsInserted(const QModelIndex &parent, int start, int end)
{
    fill(qobject_cast<GanttInfoNode*>(infoForIndex(parent)), parent, start, end);

    onAnyAddition();
}

void GanttInfoTree::onColumnsInserted(const QModelIndex &/*parent*/, int /*start*/, int /*end*/)
{
    /// TODO optimization
    reset();
}

void GanttInfoTree::onRowsRemoved(const QModelIndex &/*parent*/, int /*start*/, int /*end*/)
{
    /// TODO optimization
    reset();
}

void GanttInfoTree::onColumnsRemoved(const QModelIndex &/*parent*/, int /*start*/, int /*end*/)
{
    /// TODO optimization
    reset();
}

void GanttInfoTree::onItemAboutToBeDeleted()
{
    GanttInfoItem *item = qobject_cast<GanttInfoItem*>(QObject::sender());

    if(item)
        emit itemAboutToBeDeleted(item);
}

void GanttInfoTree::updateLimits()
{
    QPair<UtcDateTime,UtcDateTime> limits = GanttInfoItem::getLimits(_root);
    if(!limits.first.isValid() || !limits.second.isValid())
        qWarning("warning::limits not valid in updateLimits");
    qDebug()  << "updated Limits " << limits/*.first << '[' << limits.second.toString() << ']'*/;
    emit limitsChanged(limits.first,limits.second - limits.first);
}

GanttInfoItem *GanttInfoTree::lookupForVPos(int vpos, GanttInfoNode *node)
{
    bool calcItem = false;
    GanttInfoItem *foundItem = NULL;
    if(vpos < node->pos())
        return NULL;
    for(int i = 0; i < node->size(); ++i){
        if(vpos < node->at(i)->pos()){
            if(i>0)
                foundItem = node->at(i-1);   // leaf
            else{
                calcItem = true;
                foundItem = node;                   // calc
            }
            break;
        }
    }
    if(foundItem){
        GanttInfoNode *p_node = qobject_cast<GanttInfoNode*>(foundItem);
        if(p_node){
            if(calcItem)
                return p_node;
            return lookupForVPos(vpos, p_node);
        }
        GanttInfoLeaf *p_leaf = qobject_cast<GanttInfoLeaf*>(foundItem);
        if(p_leaf)
            return p_leaf;

        qCritical("ERROR unexpected");
        return NULL;
    }
    else{
        if(node->size())
            if(GanttInfoNode *p_node = qobject_cast<GanttInfoNode*>(node->at(node->size()-1)))
                return lookupForVPos(vpos,p_node);
            else return node->at(node->size()-1);
        else
            return node;
    }
}

void GanttInfoTree::fillRecursive(GanttInfoItem *item, const QModelIndex &index)
{
    if(!_model){
        qWarning("fillRecursive called reset w/o model");
        return;
    }

    for(int i = 0; i < _model->rowCount(index); ++i){
        QModelIndex childIndex = _model->index(i,0,index);
        GanttInfoItem *childItem = makeInfoItem(childIndex);
        ((GanttInfoNode*)item)->append(childItem);

        fillRecursive(childItem,childIndex); // tale recursion
    }
}

void GanttInfoTree::fill(GanttInfoNode *node, const QModelIndex &index, int from, int to)
{
    qDebug() << "fill " << node->title() << " from " << from << " to " << to;
    if(!_model){
        qWarning("fill called reset w/o model");
        return;
    }
    for(int i = from; i <= to; ++i){
        QModelIndex childIndex = _model->index(i,0,index);
        GanttInfoItem *childItem = makeInfoItem(childIndex);
        node->append(childItem);
    }
    emit rowsInserted(node, from, to);
}

GanttInfoItem *GanttInfoTree::makeInfoItem(const QModelIndex &index)
{
    if(!_model){
        qWarning("makeInfoItem called reset w/o model");
        return NULL;
    }
    GanttInfoItem *item;
    if(_model->hasChildren(index)){
        item = new GanttInfoNode(_iGanttModel->iGanttTitle(index)
                                 , _iGanttModel->iGanttStart(index)
                                 , _iGanttModel->iGanttTimeSpan(index)
                                 , index
                                 , _iGanttModel->iGanttColor(index) );
    }
    else{
        item = new GanttInfoLeaf(_iGanttModel->iGanttTitle(index)
                                 , _iGanttModel->iGanttStart(index)
                                 , _iGanttModel->iGanttTimeSpan(index)
                                 , index
                                 , _iGanttModel->iGanttColor(index) );
    }

    emit itemAdded(item);
    return item;
}

void GanttInfoTree::init()
{
    _model = NULL;
    _iGanttModel = NULL;

    _root = new GanttInfoNode(this);
    _root->setExpanded(true);
    _root->setTitle(QString("GanttInfoTree_ROOT"));

    connect(this,SIGNAL(itemAdded(GanttInfoItem*)),this,SLOT(connectNewItem(GanttInfoItem*)));
}

void GanttInfoTree::disconnectLastModel()
{
    if(_model)
        _model->disconnect(this);
}

void GanttInfoTree::connectNewModel()
{
    connect(_model, SIGNAL(modelReset()),
            this,SLOT(reset()));

    connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(onDataChanged(QModelIndex,QModelIndex)));
    connect(_model, SIGNAL(columnsInserted(QModelIndex,int,int)),
            this, SLOT(onColumnsInserted(QModelIndex,int,int)));
    connect(_model, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(onRowsInserted(QModelIndex,int,int)));

    connect(_model, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this, SLOT(onRowsRemoved(QModelIndex,int,int)));
    connect(_model, SIGNAL(columnsRemoved(QModelIndex,int,int)),
            this, SLOT(onColumnsRemoved(QModelIndex,int,int)));
}

void GanttInfoTree::connectNewItem(GanttInfoItem *item)
{
    connect(item,SIGNAL(aboutToBeDeleted()),this,SLOT(onItemAboutToBeDeleted()));

    if(GanttInfoNode *node = qobject_cast<GanttInfoNode*>(item)){
        connect(node,SIGNAL(expanded()),this,SLOT(onNodeExpanded()));
        connect(node,SIGNAL(collapsed()),this,SLOT(onNodeCollapsed()));
    }

}

void GanttInfoTree::collapseAll()
{
    for(int i = 0; i<_root->size() ; ++i)
    {
        GanttInfoNode *node = _root->nodeAt(i);
        if(node && node->isExpanded())
        {
            node->setExpanded(false);
        }
    }
}

int GanttInfoTree::heightH(GanttInfoItem *item) const
{
    if(GanttInfoNode *node = qobject_cast<GanttInfoNode*>(item)){
        if(!node->isEmpty())
            return heightH(node->at(node->size() - 1));
    }
    return item->pos() + DEFAULT_ITEM_HEIGHT;
}

void GanttInfoTree::onAnyAddition()
{
    collapseAll();
    updateLimits();
}

QAbstractItemModel *GanttInfoTree::model() const
{
    return _model;
}
