#include "mini_widget.h"

#include <QDebug>

Mini_Widget::Mini_Widget(QString borderColor, QPixmap *miniIcon, QWidget *content, QSize size, QWidget *parent) : QWidget(parent)
{
    border              = new QLabel(this);
    //example: border-color: rgba(255, 0, 0, 75%)
    border->setStyleSheet("background-color:" + borderColor + ";");
    border->setFixedSize(size.width() + 20, size.height() + 20);

    this->miniIcon      = miniIcon;
    this->content       = content;

    QLabel* pContainer  = new QLabel(this);
    pContainer->setFixedSize(size.width() - 20, size.height() - 20);
    QPixmap newPixmap = miniIcon->scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    pContainer->setPixmap(newPixmap);
    pContainer->setStyleSheet("border-radius: 20px; \
                                background-color:yellow; \
                                background-image: url(:img/mamont); \
                                background-position: top left; \
                                background-origin: content;");
//    QLabel* p           = new QLabel(this);
//    p->setText("label");
//    p->setFixedSize(this->size());

    this->setStyleSheet("border-radius: 20px;");


//    qDebug() << "mini_widget" << p->backgroundRole()<< p->foregroundRole();
}
