#include "mytabelwidgeteventfilter.h"
#include "content/content.h"

#include <QDebug>
#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QScrollBar>

#define SLIDE   1

MyTabelWidgetEventFilter::MyTabelWidgetEventFilter(QObject *parent) : QObject(parent)
{

}
bool MyTabelWidgetEventFilter::eventFilter(QObject* object, QEvent* event)
{
//    qDebug() << "EventFilter" << event->type() << " Object " << object->objectName();
    if(event->type() == QEvent::MouseButtonDblClick || \
       event->type() == QEvent::Wheel)
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
void MyTabelWidgetEventFilter::slide(QObject *object, QEvent *event)
{
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

    QTableWidget* pTable = static_cast<QTableWidget*>(object->parent());;
    QScrollBar* pScroll = nullptr;
    int difference = 0;

//горизонтальный прокрутка
    if(x > mouseEvent->x()){
        difference = mouseEvent->x() - x;
        x = mouseEvent->x();
        pScroll = pTable->horizontalScrollBar();
        pScroll->setValue(pScroll->value() + abs(difference) );
    }
    if(x < mouseEvent->x()){
        difference = mouseEvent->x() - x;
        x = mouseEvent->x();
        pScroll = pTable->horizontalScrollBar();
        pScroll->setValue(pScroll->value() - abs(difference) );
    }
//вертикальная прокрутка
    if(y > mouseEvent->y()){
        difference = mouseEvent->y() - y;
        y = mouseEvent->y();
        pScroll = pTable->verticalScrollBar();
        pScroll->setValue(pScroll->value() + abs(difference) );
    }
    if(y < mouseEvent->y()){
        difference = mouseEvent->y() - y;
        y = mouseEvent->y();
        pScroll = pTable->verticalScrollBar();
        pScroll->setValue(pScroll->value() - abs(difference) );
    }
}
