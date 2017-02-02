#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "extensions/dtline.h"

#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _dtline = new DtLine(this);
    centralWidget()->layout()->addWidget(_dtline);

    _dtline->setMin(ui->dateTimeEdit->dateTime());
    _dtline->setTimeSpan(TimeSpan(_dtline->min().dateTime().msecsTo(ui->dateTimeEdit_2->dateTime())*1000));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    _dtline->setMin(dateTime);
    _dtline->setTimeSpan(TimeSpan(_dtline->min().dateTime().msecsTo(ui->dateTimeEdit_2->dateTime())*1000));
}


void MainWindow::on_dateTimeEdit_2_dateTimeChanged(const QDateTime &dateTime)
{
    _dtline->setTimeSpan(TimeSpan(_dtline->min().dateTime().msecsTo(dateTime)*1000));
}
