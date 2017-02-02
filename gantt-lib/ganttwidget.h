#ifndef GANTTWIDGET_H
#define GANTTWIDGET_H

#include "gantt-lib_global.h"
#include "gantt/interfaces/iganttmodel.h"
#include "gantt/info/ganttinfotree.h"

#include <QModelIndex>

#include <QList>
#include <QTreeView>
#include <QWidget>


namespace Ui {
class GanttWidget;
}

class GanttScene;


class GANTTLIBSHARED_EXPORT GanttWidget : public QWidget
{
    Q_OBJECT
    void init();
public:
    explicit GanttWidget(QWidget *parent = 0);
    ~GanttWidget();


    void installEventWatcherInterval(QObject *watcher);
    void setPlayerSpeeds(qreal minMultiply, qreal maxMultiply);///< Устанавливает минимальную и максимальную скорости воспроизведения
    bool player() const;

    void setModel(IGanttModel *model);
    void setView(QTreeView *view, bool inner = false);

public slots:
    void dataReset();
    void showPlayer(bool show);


private slots:
    void onGanttViewCustomContextMenuRequested(const QPoint&);

private:
    void connectPlayer();
    void connectSceneWithInfo();
    void connectIntervals();

private:
    Ui::GanttWidget *ui;
    GanttInfoTree *_treeInfo;
    GanttScene *_scene;

    bool m_playerShown;
};

#endif // GANTTWIDGET_H
