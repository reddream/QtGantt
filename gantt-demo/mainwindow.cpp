#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dtline.h"

#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DtLine * dtline = new DtLine(this);
    dtline->setMin(UtcDateTime(2000,5,6,7,8,20));
    dtline->setTimeSpan(TimeSpan(2,4,20));

    QVBoxLayout *lay = new QVBoxLayout();
    lay->addWidget(dtline);
    centralWidget()->setLayout(lay);

}

MainWindow::~MainWindow()
{
    delete ui;
}
