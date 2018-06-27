#include "mytabelwidgeteventfilter.h"

#include <QDebug>
#include <QObject>

MyTabelWidgetEventFilter::MyTabelWidgetEventFilter(QObject *parent) : QObject(parent)
{

}
bool MyTabelWidgetEventFilter::eventFilter(QObject* object, QEvent* event)
{
//    qDebug() << "EventFilter" << event->type() << " Object " << object->objectName();
    if(event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick || \
       event->type() == QEvent::MouseButtonRelease || event->type() == QEvent::MouseMove)
        return true;
    return false;
}
