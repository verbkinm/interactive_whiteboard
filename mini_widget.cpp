#include "mini_widget.h"
#include <QLabel>

#include <QDebug>

Mini_Widget::Mini_Widget(QWidget *parent) : QWidget(parent)
{
    QLabel* p = new QLabel(this);
    p->setText("label");
    p->setFixedSize(this->size());


    qDebug() << "mini_widget" << p->backgroundRole()<< p->foregroundRole();
}
