#include "myviewereventfilter.h"

#include <QEvent>
#include <QMouseEvent>
#include <QScrollArea>
#include <QScrollBar>

#include <QDebug>

MyViewerEventFilter::MyViewerEventFilter(QObject *parent) : QObject(parent)
{

}
bool MyViewerEventFilter::eventFilter(QObject* object, QEvent* event)
{
    if(event->type() == QEvent::MouseButtonDblClick || \
       event->type() == QEvent::Wheel || \
       event->type() == QEvent::MouseButtonRelease)
        return true;

    if(event->type() == QEvent::MouseButtonPress){
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        x = mouseEvent->x();
        y = mouseEvent->y();
    }

    if(event->type() == QEvent::MouseMove){
        slide(object, event);

        return true;
    }

    return false;
}
void MyViewerEventFilter::slide(QObject *object, QEvent *event)
{
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

    QScrollArea* pScrollArea = static_cast<QScrollArea*>(object->parent());;
    QScrollBar* pScroll = nullptr;
    int difference = 0;

//горизонтальный прокрутка
    if(x > mouseEvent->x()){
        difference = mouseEvent->x() - x;
        x = mouseEvent->x();
        pScroll = pScrollArea->horizontalScrollBar();
        qDebug() << pScroll->value();
        pScroll->setValue(pScroll->value() + abs(difference) );
    }
    if(x < mouseEvent->x()){
        difference = mouseEvent->x() - x;
        x = mouseEvent->x();
        pScroll = pScrollArea->horizontalScrollBar();
        pScroll->setValue(pScroll->value() - abs(difference) );
    }
//вертикальная прокрутка
    if(y > mouseEvent->y()){
        difference = mouseEvent->y() - y;
        y = mouseEvent->y();
        pScroll = pScrollArea->verticalScrollBar();
        pScroll->setValue(pScroll->value() + abs(difference) );
    }
    if(y < mouseEvent->y()){
        difference = mouseEvent->y() - y;
        y = mouseEvent->y();
        pScroll = pScrollArea->verticalScrollBar();
        pScroll->setValue(pScroll->value() - abs(difference) );
    }
}
