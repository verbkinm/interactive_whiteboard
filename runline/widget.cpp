#include "widget.h"

#include <QtWidgets>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    x1 = this->width();
    x2 = 0;
//    this->setWindow(800,100);

    connect(&timer, SIGNAL(timeout()), this, SLOT(slotRunLine()));
    timer.start(25);

    str = "DRAW TEXT";
    str_tail = "\0";

    font = new QFont("Arial", 25, QFont::Normal);
    fontMetric = new QFontMetrics(*font);

    stringLenght = fontMetric->width(str);
}
void Widget::slotRunLine()
{
    x1 -= 1;
    x2 -= 1;
    repaint();
}
void Widget::paintEvent(QPaintEvent *event)
{
    if( x1 == 0)
    {
        x2 = this->width();
        str_tail = str;
//        x = this->width();
    }

    x_end = x1 + stringLenght;
    if(x_end == 0)
    {
        x1 = x2;
        str_tail.clear();
    }

    QPainter painter(this);

    painter.setFont(*font);

    painter.drawText(x1,0, this->width(), this->height(), Qt::AlignVCenter, str);
    painter.drawText(x2,0, this->width(), this->height(), Qt::AlignVCenter, str_tail);
}
bool Widget::event(QEvent *event)
{

    return QWidget::event(event);
}
Widget::~Widget()
{

}
