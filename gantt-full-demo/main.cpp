#include "mainwindow.h"
#include "appeventfilter.h"
#include <QApplication>
#include "QTextCodec"

int main(int argc, char *argv[])
{

    QTextCodec *tc = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(tc);
    QTextCodec::setCodecForLocale(tc);
    QTextCodec::setCodecForTr(tc);
    QApplication a(argc, argv);


//    a.installEventFilter(new AppEventFilter());

    MainWindow w;

    w.show();

    return a.exec();
}
