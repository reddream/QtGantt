#include "appeventfilter.h"

#include <QEvent>
#include <QAbstractSpinBox>
#include <QComboBox>

#include <QDebug>

AppEventFilter::AppEventFilter(QObject *parent) : QObject(parent)
{

}

bool AppEventFilter::eventFilter(QObject *object, QEvent *event)
{
    qDebug() << "event from ["
            <<object
            <<"] event :["
            <<event
            <<']';
    return QObject::eventFilter(object,event);
}

