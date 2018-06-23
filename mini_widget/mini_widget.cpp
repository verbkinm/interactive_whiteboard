#include "mini_widget.h"
#include "widget_types/schedule.h"

#include <QDebug>
#include <QEvent>
#include <QPaintEvent>
#include <QPainter>

Mini_Widget::Mini_Widget(QString borderColor, int borderWidth, QString borderClickColor, int borderClickWidth, \
                         QPixmap *miniIcon, QSize size, QString type, \
                         QString textColor, QString backgroudColor, QWidget *parent) : QWidget(parent)
{
    myType = type;
    mySize = size;

    this->borderWidth           = borderWidth;
    this->borderClickWidth      = borderClickWidth;
    this->borderColor           = borderColor;
    this->borderClickColor      = borderClickColor;

    this->textColor             = textColor;
    this->backgroudColor        = backgroudColor;

//рамка, которая будет появлятся при нажатии
    borderClick              = new QLabel(this);
    //example: border-color: rgba(255, 0, 0, 75%)
    borderClick->setStyleSheet("background-color:" + borderClickColor + ";");
    borderClick->setFixedSize(size.width() + borderClickWidth, size.height() + borderClickWidth);
    borderClick->hide();

//рамка
    border              = new QLabel(this);
    //example: border-color: rgba(255, 0, 0, 75%)
    border->setStyleSheet("background-color:" + borderColor + ";");
    border->setFixedSize(size.width() + borderWidth, size.height() + borderWidth);
    border->move(borderClickWidth / 2 - borderWidth / 2, borderClickWidth / 2 - borderWidth / 2);

    this->miniIcon      = miniIcon;
//    this->content       = content;


    selectTypeWidget();


    this->setStyleSheet("border-radius: 20px;");
    this->setFixedSize(borderClick->size());
}
void Mini_Widget::selectTypeWidget()
{
    pContent = 0;
    if(myType == "label")
        createLabelForMiniWidget();
    else if(myType == "clock"){
        centralWidget = new QWidget(this);
        centralWidget->move(borderClickWidth / 2, borderClickWidth/2);
            Clock* pClock = new Clock(textColor, backgroudColor, centralWidget);
        pClock->setFixedSize(mySize);
    }
    else if(myType == "schedule")
    {
        createLabelForMiniWidget();
        pContent = new Content;
        Schedule* pSchedule = new Schedule;
        pContent->addWidget(pSchedule);
    }
}
void Mini_Widget::createLabelForMiniWidget()
{
    centralLabel = new QLabel(this);
    centralLabel->setFixedSize(mySize);
    centralLabel->move(borderClickWidth / 2, borderClickWidth/2);

    QPixmap* newPixmap = new QPixmap;
    *newPixmap = miniIcon->scaled(mySize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    centralLabel->setPixmap(*newPixmap);
}
void Mini_Widget::paintEvent(QPaintEvent *event)
{

}
bool Mini_Widget::event(QEvent *event)
{
//    qDebug() << event->type();
    if(event->type() == QEvent::MouseButtonPress){
        borderClick->setVisible(true);
    }
    if(event->type() == QEvent::MouseButtonRelease){
        borderClick->setVisible(false);
        if(pContent != 0)
            pContent->showFullScreen();
    }

    return QWidget::event(event);
}
