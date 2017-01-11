#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dtline.h"

#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _dtline = new DtLine(this);
    centralWidget()->layout()->addWidget(_dtline);

    _dtline->setMin(ui->dateTimeEdit->dateTime());
    qDebug() << "min "<< _dtline->min();
    qDebug() << "ts " << QString::number(_dtline->min().dateTime().msecsTo(ui->dateTimeEdit_2->dateTime())*1000);
    _dtline->setTimeSpan(TimeSpan(_dtline->min().dateTime().msecsTo(ui->dateTimeEdit_2->dateTime())*1000));
    qDebug() << "min "<< _dtline->min() << "timespan " << _dtline->timeSpan().toString();
    qDebug() << "max " << _dtline->max();
//    exit(0);
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
    qDebug() << "new dt "<< dateTime;
    _dtline->setTimeSpan(TimeSpan(_dtline->min().dateTime().msecsTo(dateTime)*1000));
}
