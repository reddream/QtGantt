//#include "gantttreeview.h"
////#include "ganttheaderview.h"

//#include "gantt-lib_global_values.h"
//#include "ganttgraphicsview.h"
//#include "gantttreedelegate.h"
//#include "gantttreemodel.h"


//#include <QScrollBar>
//#include <QMouseEvent>

//#include <QDebug>

//GanttTreeView::GanttTreeView(QWidget * parent )
//    : QTreeView(parent)
//{
//    m_graphicsView = NULL;
//    m_model = NULL;
//    m_hSliderHeight = 15;

////    m_header = new GanttHeaderView(Qt::Horizontal,this);
////    setHeader(m_header);

//    setMinimumWidth(GANTTTREEVIEW_MIN_WIDTH);
//    horizontalScrollBar()->setStyleSheet(
//                QString("QScrollBar {height:%1px;}").arg(m_hSliderHeight));
//    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    setMouseTracking(true);

//    setSelectionBehavior(QAbstractItemView::SelectRows);
////    setSelectionMode(QAbstractItemView::NoSelection);
//    setFrameStyle(0);



////    setItemDelegateForColumn(1, new GanttTreeDelegate(this));
////    setItemDelegateForColumn(2, new GanttTreeDelegate(this));

//}

//void GanttTreeView::scrollContentsBy(int dx, int dy)
//{

//    if(dx)
//        repaintHeader();

//    if(!m_graphicsView)
//        return;
//    int vs = verticalScrollBar()->value();
//    m_graphicsView->verticalScrollBar()->setValue(vs);
//    if(m_graphicsView->verticalScrollBar()->value() != vs)
//        verticalScrollBar()->setValue(m_graphicsView->verticalScrollBar()->value());

//    QList<QRectF> rects;
//    rects.append(m_graphicsView->viewport()->rect());
//    m_graphicsView->updateScene(rects);


//    QTreeView::scrollContentsBy(dx,dy);
//}

//void GanttTreeView::mouseMoveEvent(QMouseEvent *event)
//{
//    QPoint pos = event->pos();

//    if( pos.y() > rect().bottom() - header()->height() - m_hSliderHeight)
//    {
//        setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
//    }
//    else
//    {
//        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    }
//    QTreeView::mouseMoveEvent(event);
//}

//void GanttTreeView::leaveEvent(QEvent *e)
//{
//    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

//    QTreeView::leaveEvent(e);
//}

//void GanttTreeView::updateVisibilityHelper(const GanttInfoNode *node)
//{
//    if(!node)
//        return;

//    if(!node->size())
//    {
//        setRowHidden(node->row(),node->parent()->index(), true);
//        return;
//    }

//    setRowHidden(node->row(),node->parent()->index(), false);

//    for(int i = 0; i < node->size(); ++i)
//    {
//        updateVisibilityHelper(node->nodeAt(i));
//    }
//}

//void GanttTreeView::collapseNode(GanttInfoNode *node)
//{
//    if(node)
//    {
//        collapse(node->index());
//        emit collapsed(node->index());
//    }
//}


//void GanttTreeView::setGraphicsView(GanttGraphicsView *graphicsView)
//{

//    m_graphicsView = graphicsView;
//}

//void GanttTreeView::setModel(QAbstractItemModel *model)
//{
//    m_model = dynamic_cast<GanttTreeModel*>(model);
////    updateVisibility();

////    connect(m_model,SIGNAL(itemsAdded()),this,SLOT(updateVisibility()));
////    connect(m_model,SIGNAL(itemsAdded(GanttInfoItem*)),this,SLOT(updateItemVisibility(const GanttInfoItem*)));
//    connect(m_model,SIGNAL(needCollapse(GanttInfoNode*)),this,SLOT(collapseNode(GanttInfoNode*)));

//    QTreeView::setModel(model);
//}

//void GanttTreeView::setCurrentIndex(const QModelIndex &index)
//{
//    QAbstractItemView::setCurrentIndex(index);
//}


//void GanttTreeView::repaintHeader()
//{
//    header()->reset();
//}

//void GanttTreeView::updateVisibility()
//{
//    if(!m_model)
//        return;
//    GanttInfoNode *root = m_model->root();

//    for(int i = 0; i < root->size(); ++i)
//    {
//        updateVisibilityHelper(root->nodeAt(i));
//    }
//}


//void GanttTreeView::updateItemVisibility(const GanttInfoItem *item)
//{
//    const GanttInfoNode *node;
//    if(item)
//        node = qobject_cast<const GanttInfoNode*>(item);
//    else
//        node = qobject_cast<const GanttInfoNode*>(sender());

//    if(node)
//    {
//        updateVisibilityHelper(node);
//    }
//}
