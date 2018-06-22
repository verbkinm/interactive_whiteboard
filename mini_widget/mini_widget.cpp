#include "mini_widget.h"

#include <QDebug>
#include <QEvent>
#include <QPaintEvent>
#include <QPainter>

Mini_Widget::Mini_Widget(QString borderColor, int borderWidth, QString borderClickColor, int borderClickWidth, QPixmap *miniIcon, Content *content, QSize size, QWidget *parent) : QWidget(parent)
{
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
    this->content       = content;

    QPixmap* newPixmap = new QPixmap;
    *newPixmap = miniIcon->scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    centralLabel = new QLabel(this);
    centralLabel->setFixedSize(size);
    centralLabel->move(borderClickWidth / 2, borderClickWidth/2);
    centralLabel->setPixmap(*newPixmap);

    this->setStyleSheet("border-radius: 20px;");
    this->setFixedSize(borderClick->size());
}
void Mini_Widget::paintEvent(QPaintEvent *event)
{

}
bool Mini_Widget::event(QEvent *event)
{
    qDebug() << event->type();
    if(event->type() == QEvent::MouseButtonPress){
        borderClick->setVisible(true);
    }
    if(event->type() == QEvent::MouseButtonRelease){
        borderClick->setVisible(false);
        content->showFullScreen();
    }

    return QWidget::event(event);
}
