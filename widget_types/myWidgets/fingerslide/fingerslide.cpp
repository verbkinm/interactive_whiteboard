#include "fingerslide.h"
#include "content/content.h"

#include <QDebug>
#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QScrollBar>

#define SLIDE   1

FingerSlide::FingerSlide(QObject *parent) : QObject(parent)
{
    object = parent;
}
bool FingerSlide::eventFilter(QObject* object, QEvent* event)
{
    if(event->type() == QEvent::MouseButtonDblClick || \
       event->type() == QEvent::Wheel)
        return true;

    if(event->type() == QEvent::MouseButtonPress){
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        preX = x = mouseEvent->x();
        preY = y = mouseEvent->y();
        Tt.stop();
        Ts.stop();
    }

    if(event->type() == QEvent::MouseMove){
        slide(object, event);
        return true;
    }

    if(event->type() == QEvent::MouseButtonRelease){

        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        stepX = preX - mouseEvent->x();
        stepY = preY - mouseEvent->y();

        connect(&Tt, SIGNAL(timeout()), &Ts, SLOT(stop()));
        connect(&Ts, SIGNAL(timeout()), this, SLOT(slotInertion()));

        Tt.start(300);
        Ts.start(20);


        return true;
    }

    return false;
}
void FingerSlide::slide(QObject *object, QEvent *event)
{
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

    QAbstractScrollArea* pScrollArea = static_cast<QAbstractScrollArea*>(object->parent());;
    QScrollBar* pScroll = nullptr;

    int differenceX, differenceY;

    preX = x;
    preY = y;

//горизонтальный прокрутка
    if(x > mouseEvent->x()){
        differenceX = mouseEvent->x() - x;
        x = mouseEvent->x();
        pScroll = pScrollArea->horizontalScrollBar();
        pScroll->setValue(pScroll->value() + abs(differenceX) );
    }
    if(x < mouseEvent->x()){
        differenceX = mouseEvent->x() - x;
        x = mouseEvent->x();
        pScroll = pScrollArea->horizontalScrollBar();
        pScroll->setValue(pScroll->value() - abs(differenceX) );
    }
//вертикальная прокрутка
    if(y > mouseEvent->y()){
        differenceY = mouseEvent->y() - y;
        y = mouseEvent->y();
        pScroll = pScrollArea->verticalScrollBar();
        pScroll->setValue(pScroll->value() + abs(differenceY) );
    }
    if(y < mouseEvent->y()){
        differenceY = mouseEvent->y() - y;
        y = mouseEvent->y();
        pScroll = pScrollArea->verticalScrollBar();
        pScroll->setValue(pScroll->value() - abs(differenceY) );
    }
}
void FingerSlide::slotInertion()
{
    QAbstractScrollArea* pScrollArea = static_cast<QAbstractScrollArea*>(object->parent());;
    QScrollBar* pScrollX = pScrollArea->horizontalScrollBar();
    QScrollBar* pScrollY = pScrollArea->verticalScrollBar();

    pScrollX->setValue(pScrollX->value() + stepX/20 );
    pScrollY->setValue(pScrollY->value() + stepY/20 );
}

FingerSlide::~FingerSlide()
{

}
