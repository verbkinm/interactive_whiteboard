#include<QPainter>
#include <QEvent>
#include <QDebug>

#include "content.h"

Content::Content(QWidget *parent) : QLabel(parent)
{
    pLayout = new QVBoxLayout();
    this->setLayout(pLayout);
}
void Content::addWidget(QWidget *w)
{
    pLayout->addWidget(w);
}
bool Content::event(QEvent *event)
{
//    qDebug() << "Content" << event->type();
    if(event->type() == QEvent::MouseButtonRelease /*|| event->type() == QEvent::TouchEnd*/)
        this->close();

    return QWidget::event(event);
}

