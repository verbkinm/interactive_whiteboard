#include "main_widget.h"
#include <QDebug>

Main_Widget::Main_Widget()
    : QWidget(), widget_settings("LYCEUM","interactive_whiteboard")
{
    Mini_Widget* pmini = new Mini_Widget(this);
    pmini->setStyleSheet("background-color:blue;");
    pmini->move(100,100);
    pmini->show();

    QWidget* p = new QWidget(this);
    p->setStyleSheet("background-color:red;");
    p->move(50,50);
    p->show();

    qDebug() << pmini->size() << pmini->geometry();

    this->setStyleSheet("background-color:green;");
}

Main_Widget::~Main_Widget()
{

}
