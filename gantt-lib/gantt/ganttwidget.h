#ifndef GANTTWIDGET_H
#define GANTTWIDGET_H

#include "gantt-lib_global.h"
#include "iganttmodel.h"
#include "ganttinfotree.h"

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

public:
    explicit GanttWidget(QWidget *parent = 0);
    ~GanttWidget();


    void setPlayerSpeeds(qreal minMultiply, qreal maxMultiply);///< Устанавливает минимальную и максимальную скорости воспроизведения
    void showPlayer(bool show);
    bool player() const;

    void setModel(IGanttModel *model);
    void setView(QTreeView *view, bool inner = false);





signals:

private slots:

    void onGanttViewCustomContextMenuRequested(const QPoint&);

public slots:



private:
    void init();

private:
    Ui::GanttWidget *ui;
    GanttInfoTree *_treeInfo;


    bool m_playerShown;

};

#endif // GANTTWIDGET_H
