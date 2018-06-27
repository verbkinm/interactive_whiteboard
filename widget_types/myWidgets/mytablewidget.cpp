#include "mytablewidget.h"

#include <QEvent>
#include <QDebug>

MyTableWidget::MyTableWidget()
{

}
bool MyTableWidget::event(QEvent *event)
{
//    qDebug() << "MYTABLE WIDGET " <<event->type();
    return QWidget::event(event);
}
