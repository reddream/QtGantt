#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "utcdatetime.h"
#include "gantttreemodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    void init();
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


protected:
    void keyPressEvent(QKeyEvent *e);

private:
    Ui::MainWindow *ui;
    GanttTreeModel *_model;

private slots:
    void on_pushButton_ganttAddItems_clicked();
    void on_pushButton_ganttClear_clicked();
    void on_radioButton_ganttPlayer_toggled(bool checked);
};

#endif // MAINWINDOW_H
