#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <QKeyEvent>
#include "utcdatetime.h"
#include "gantt-lib_global_values.h"
#include "gantttreemodel.h"


void MainWindow::init()
{
    _model = new GanttTreeModel();
    ui->treeView->setModel(_model);
    ui->ganttWidget->installEventWatcherInterval(this);

    ui->ganttWidget->setModel(_model);
    ui->ganttWidget->setView(ui->treeView);
}

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();

    qDebug() << "test data initialized";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{

    QMainWindow::keyPressEvent(e);
}

QList<GanttInfoItem*> generateTest()
{
    static int kk = 0;
    QList<GanttInfoItem*> testList;
    for(int i = kk; i<kk+500; ++i)
    {
        GanttInfoNode *node = new GanttInfoNode;

        node->setTitle("node"+QString::number(i));
        int year = 2014 + qrand()%2,
                month = 1 + qrand()%3,
                day = 1 + qrand()%2,
                hour = qrand()%2,
                minute = qrand()%60,
                sec = qrand()%60,
                microsec = qrand()%1000000;

        node->setStart(UtcDateTime(year,month,day,hour,minute,sec,microsec));

        QColor color = QColor::fromRgb(qrand()%255,qrand()%255,qrand()%255);
        int max = qrand()%4;

        for(int j = 0; j<max; ++j)
        {
            GanttInfoLeaf *leaf = new GanttInfoLeaf;

            int year = 2016,
                    month = 1 /*+ qrand()%6*/,
                    day = 1 + qrand()%2,
                    hour = qrand()%2,
                    minute = qrand()%60,
                    sec = qrand()%60,
                    microsec = qrand()%1000000;

            UtcDateTime start(year,month,day,hour,minute,sec,microsec),
                    finish = start
                        .addMicroseconds((hour * SECONDS_IN_HOUR * _MICROSECONDS_IN_SECOND)
                                         + minute * SECONDS_IN_MINUTE * _MICROSECONDS_IN_SECOND
                                         + sec * _MICROSECONDS_IN_SECOND
                                         + microsec)
                        .addDays(day )
                        .addMonths(month );

            leaf->setStart(start);
            leaf->setTimeSpan(finish - start);
            leaf->setColor(color);
            leaf->setTitle("leaf"+QString::number(1 + i) + ':' + QString::number(1 + j));

            node->append(leaf);
        }


        testList.append(node);
    }
    kk+=10;
    return testList;
}

void MainWindow::on_pushButton_ganttAddItems_clicked()
{
    _model->addItems(generateTest());
}

void MainWindow::on_pushButton_ganttClear_clicked()
{
    _model->clear();
}

void MainWindow::on_radioButton_ganttPlayer_toggled(bool checked)
{
    ui->ganttWidget->showPlayer(checked);
}

