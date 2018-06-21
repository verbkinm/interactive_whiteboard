#include "main_widget.h"
#include <QDebug>

Main_Widget::Main_Widget()
    : QWidget(), widget_settings("LYCEUM","interactive_whiteboard")
{


    QWidget* p = new QWidget();
    p->setParent(this);
    p->setStyleSheet("background-color:red;");
    p->move(50,50);
    p->show();

    QPixmap* pixmap = new QPixmap(":img/mamont");
    Mini_Widget* pmini = new Mini_Widget("rgba(100, 100, 100, 50%)", pixmap, new QWidget, QSize(100,100), this);
    pmini->move(100,100);
    pmini->show();


    this->setStyleSheet("background-color:green;");

    qDebug() << "widget" << p->backgroundRole()<< p->foregroundRole();
    qDebug() << "this" << backgroundRole()<< foregroundRole();
}

Main_Widget::~Main_Widget()
{

}
