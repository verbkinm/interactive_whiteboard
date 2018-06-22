#include<QPainter>
#include <QEvent>

#include "content.h"



Content::Content(QString type, QWidget *parent) : QLabel(parent)
{
    this->setStyleSheet("border-radius: 20px;");


    if(type == "clock"){
        pClock = new Clock;
        pClock->show();
    }

}
bool Content::event(QEvent *event)
{
    if(event->type() == QEvent::MouseButtonRelease /*|| event->type() == QEvent::TouchEnd*/)
        this->hide();

    return QWidget::event(event);
}
//void Content::paintEvent(QPaintEvent *event)
//{
//    QPainter painter(this);
//    painter.setPen(QPen(Qt::black,1,Qt::SolidLine)); // Настройки рисования
//    painter.drawLine(0,0,width(),height()); // Рисование линии

//    QPixmap newPixmap = miniIcon->scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    if(!emptyPixmap)
//        painter.drawPixmap(0,0, 50,50, *pPixmap);
//}
