#include "ganttwidget.h"
#include "ui_ganttwidget.h"

#include "ganttscene.h"

#include <QScrollBar>
#include <QAbstractItemModel>


GanttWidget::GanttWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GanttWidget)
{
    ui->setupUi(this);
    init();

    ui->playerControl->hide();
    ui->playerSettings->hide();

    if(layout())
    {
        layout()->setMargin(0);
        layout()->setSpacing(0);
    }

    ui->treeView->setContentsMargins(0,0,0,0);
    ui->treeView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->treeView->verticalScrollBar()->setStyleSheet("QScrollBar {width:0px;}");

    installEventFilter(ui->widgetIntervalSlider);

    ui->playerControl->setSettings(ui->playerSettings);

}

GanttWidget::~GanttWidget()
{
    delete ui;
}

void GanttWidget::setPlayerSpeeds(qreal minMultiply, qreal maxMultiply)
{
    ui->playerSettings->setMultiplies(minMultiply,maxMultiply);
}

void GanttWidget::showPlayer(bool show)
{
    m_playerShown = show;
    ui->playerControl->setVisible(show);
    ui->playerSettings->setVisible(show);

//    if(m_scene)
//        m_scene->setDrawCurrentDtSlider(show);

    update();
}

bool GanttWidget::player() const
{
    return m_playerShown;
}

void GanttWidget::setModel(IGanttModel *model)
{
    _treeInfo->setModel(model);

    /// TODO remove
    setView(ui->treeView);
}

void GanttWidget::setView(QTreeView *view, bool inner)
{
    /// TODO
    if(!_treeInfo || !_treeInfo->model())
        return;

    view->setModel(_treeInfo->model());
    _treeInfo->connectTreeView(view);
}

void GanttWidget::onGanttViewCustomContextMenuRequested(const QPoint &point)
{
    QPoint widgetPoint =ui->ganttView->mapTo(this,point);

    emit customContextMenuRequested(point);
}

void GanttWidget::init()
{
    _treeInfo = new GanttInfoTree(this);
    _scene = new GanttScene(ui->ganttView,ui->widgetDtLine,this);
    connectSceneWithInfo();
    connectIntervals();
}

void GanttWidget::connectSceneWithInfo()
{
    _scene->setTreeInfo(_treeInfo);

//    connect(_scene,SIGNAL(currentItemChanged(const GanttInfoItem*)),_treeInfo,SLOT(onCurrent));

    connect(_treeInfo,SIGNAL(currentChanged(GanttInfoItem*)),_scene,SLOT(setCurrentByInfo(GanttInfoItem*)));
    connect(_treeInfo,SIGNAL(itemAdded(GanttInfoItem*)),_scene,SLOT(onItemAdded(GanttInfoItem*)));
    connect(_treeInfo,SIGNAL(itemAboutToBeDeleted(GanttInfoItem*)),_scene,SLOT(onItemRemoved(GanttInfoItem*)));

    connect(_treeInfo,SIGNAL(endInsertItems()),_scene,SLOT(onEndInsertItems()));
    connect(_treeInfo,SIGNAL(endRemoveItems()),_scene,SLOT(onEndRemoveItems()));

}

void GanttWidget::connectIntervals()
{   //DtLine shows a bit more than GanttIntervalSlider
    connect(_treeInfo,SIGNAL(limitsChanged(UtcDateTime,TimeSpan)),ui->widgetIntervalSlider,SLOT(setLimits(UtcDateTime,TimeSpan)));
    connect(_treeInfo,SIGNAL(limitsChanged(UtcDateTime,TimeSpan)),ui->widgetDtLine,SLOT(setLimitsWithOffset(UtcDateTime,TimeSpan)));

    connect(ui->widgetIntervalSlider,SIGNAL(rangeChangedManually(UtcDateTime,TimeSpan)),
                ui->widgetDtLine,SLOT(setLimitsWithOffset(UtcDateTime,TimeSpan)));
    connect(ui->widgetDtLine,SIGNAL(changedManually(UtcDateTime,TimeSpan)),
                ui->widgetIntervalSlider,SLOT(setRange(UtcDateTime,TimeSpan)));
}

