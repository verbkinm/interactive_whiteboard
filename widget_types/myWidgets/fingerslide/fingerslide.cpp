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

}
bool FingerSlide::eventFilter(QObject* object, QEvent* event)
{
    if(event->type() == QEvent::MouseButtonDblClick || \
       event->type() == QEvent::Wheel)
        return true;

    if(event->type() == QEvent::MouseButtonPress){
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        preX = x = mouseEvent->x();
        y = mouseEvent->y();
        time.start();

//        if(tStep.isActive()){
//            tStep.stop();
//            disconnect(&tStep, SIGNAL(timeout()), this, SLOT(slotInertion()) );
//            this->object = nullptr;
//            this->event  = nullptr;
//        }
    }

    if(event->type() == QEvent::MouseMove){
        slide(object, event);
        return true;
    }

    if(event->type() == QEvent::MouseButtonRelease){

//        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

//        qDebug() << "t - " << t;
//        S = abs(abs(mouseEvent->x()) - abs(preX));
//        qDebug() << "S - " << S;

//        if(S != 0){
//            V = float(S) / t;
//            qDebug() << "V - " << V;
//            if(V > 0.08){
//                tStep.start(t);
//                this->object = object;
//                this->event = event;
//                connect(&tStep, SIGNAL(timeout()), this, SLOT(slotInertion()) );
//            }
//        }
//        V = -1  ;

        return false;
    }

    return false;
}
void FingerSlide::slide(QObject *object, QEvent *event)
{
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

    QAbstractScrollArea* pScrollArea = static_cast<QAbstractScrollArea*>(object->parent());;
    QScrollBar* pScroll = nullptr;

    int differenceX, differenceY;

    t = time.elapsed();
    time.start();


    preX = x;

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
    static int tEnd = 0;
    tEnd += t;
    if( tEnd >= t*100){
        tStep.stop();
        disconnect(&tStep, SIGNAL(timeout()), this, SLOT(slotInertion()) );
        tEnd = 0;
        V=0;
    }

    QAbstractScrollArea* pScrollArea = static_cast<QAbstractScrollArea*>(this->object->parent());;
    QScrollBar* pScroll = nullptr;

    pScroll = pScrollArea->horizontalScrollBar();
    pScroll->setValue( pScroll->value() + V);
    V += 1;
}

FingerSlide::~FingerSlide()
{

}
