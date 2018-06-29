#include<QPainter>
#include <QEvent>
#include <QDebug>

#include "content.h"

Content::Content(QString title, QWidget *parent) : QLabel(parent)
{
    pLayout = new QVBoxLayout();
    this->setLayout(pLayout);

    pTitle = new QLabel(title);
    pTitle->setAlignment(Qt::AlignCenter);
    pLayout->addWidget(pTitle);


}
void Content::addWidget(QWidget *w)
{
    pWidget = w;
    pLayout->addWidget(w);
// снимаем фокус с pWidget
    this->setFocus();
}
void Content::setTextSize(const unsigned int &textSize)
{
    pTitle->setStyleSheet("font-size:" + QString::number(textSize + float(textSize) / 100 * 50 ) + "px;");
}
bool Content::event(QEvent *event)
{
//    qDebug() << "Content" << event->type();
    if(event->type() == QEvent::Close)
        emit signalClose();

    if(event->type() == QEvent::MouseButtonRelease /*|| event->type() == QEvent::TouchEnd*/)
        this->close();

    return QWidget::event(event);
}
void Content::childEvent (QChildEvent* event)
{
//    qDebug() << "CHILD EVENT " << event->type();
}
void Content::mousePressEvent (QMouseEvent* event)
{
//    qDebug() << "MOUSE EVENT " << event->type();
}
void Content::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

// ИСПРАВИТЬ!!! фон из настроек
    QPixmap backgoundImage(":img/school2");
    QPixmap newPix = backgoundImage.scaled(this->size(), Qt::IgnoreAspectRatio);
    painter.setBrush(QBrush(Qt::black, newPix));
    painter.drawRect(this->rect());
}
Content::~Content(){
//    delete pWidget;
}
