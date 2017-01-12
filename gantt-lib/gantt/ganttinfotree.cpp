#include "ganttinfotree.h"

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

GanttInfoItem *GanttInfoTree::root() const
{
    return _root;
}

GanttInfoItem *GanttInfoTree::itemForIndex(const QModelIndex &index, GanttInfoItem *item) const
{
    GanttInfoLeaf *leaf = dynamic_cast<GanttInfoLeaf*>(item);
    GanttInfoNode *node = dynamic_cast<GanttInfoNode*>(item);
    if(!item)
        node = _root;
    if(leaf && leaf->index() == index)
        return leaf;
    if(node){
        if(node->index() == index)
            return node;
        else
            for(int i = 0; i < node->size(); ++i){
                GanttInfoItem *itemForIndexChild = itemForIndex(index, node->at(i));
                if(itemForIndexChild)
                    return itemForIndexChild;
            }
    }
    return NULL;
}

void GanttInfoTree::onClicked(const QModelIndex &index)
{
    /// TODO
}

void GanttInfoTree::onExpanded(const QModelIndex &index)
{
    GanttInfoNode *node = dynamic_cast<GanttInfoNode*>(itemForIndex(index));
    if(node)
        node->setExpanded(true);
}

void GanttInfoTree::onCollapsed(const QModelIndex &index)
{
    GanttInfoNode *node = dynamic_cast<GanttInfoNode*>(itemForIndex(index));
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
    emit endInsertItems();
}

void GanttInfoTree::onDataChanged(const QModelIndex &from, const QModelIndex &to)
{
    /// TODO: optimization
    reset();
}

void GanttInfoTree::fillRecursive(GanttInfoItem *item, const QModelIndex &index)
{
    if(!_model){
        qWarning("fillInfoByIndex called reset w/o model");
        return;
    }
    if(_model->hasChildren(index)){
        for(int i = 0; i < _model->rowCount(index); ++i){
            QModelIndex childIndex = _model->index(i,0,index);
            GanttInfoItem *item = makeInfoItem(childIndex);
            fillRecursive(item,childIndex);
            ((GanttInfoNode*)item)->append(item);
        }
    }
}

GanttInfoItem *GanttInfoTree::makeInfoItem(const QModelIndex &index)
{
    if(!_model){
        qWarning("makeInfoItem called reset w/o model");
        return NULL;
    }
    GanttInfoItem *item;
    if(_model->hasChildren(index)){
        item = new GanttInfoNode(_iGanttModel->title(index)
                                 , _iGanttModel->start(index)
                                 , _iGanttModel->finish(index)
                                 , index
                                 , _iGanttModel->color(index) );
    }
    else{
        item = new GanttInfoLeaf(_iGanttModel->title(index)
                                 , _iGanttModel->start(index)
                                 , _iGanttModel->finish(index)
                                 , index
                                 , _iGanttModel->color(index) );
    }
    emit itemAdded(item);
    return item;
}

void GanttInfoTree::init()
{
    _root = new GanttInfoNode(this);
}

void GanttInfoTree::disconnectLastModel()
{
    if(_model)
        _model->disconnect(this);
}

void GanttInfoTree::connectNewModel()
{
    connect(_model,SIGNAL(modelReset()),this,SLOT(reset()));
    connect(_model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(onDataChanged(QModelIndex,QModelIndex)));
}
