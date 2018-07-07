#include<QPainter>
#include <QEvent>
#include <QDebug>
#include <QHBoxLayout>
#include <QTimer>

#include "content.h"

Content::Content(QString title, QString backgoundImagePath, int timerSec, QWidget *parent) : QLabel(parent)
{
    timerSec *= 1000;

    this->backgoundImage = new QPixmap(backgoundImagePath);

    pLayout = new QVBoxLayout();
    this->setLayout(pLayout);

    pTitle = new QLabel(title);
    pTitle->setAlignment(Qt::AlignCenter);
    pLayout->addWidget(pTitle);


    home.setText("Закрыть");
    connect(&home, SIGNAL(clicked(bool)), this, SLOT(close()));

//таймер бездействия
    connect(&timer, SIGNAL(timeout()), this, SLOT(close()));
    timer.start(timerSec);

    this->timerSec = timerSec;
}
void Content::addWidget(QWidget *w)
{
    pWidget = w;
    pLayout->addWidget(w);

    pHLayout = new QHBoxLayout;
    pHLayout->addStretch(1);
    pHLayout->addWidget(&home);
    pHLayout->addStretch(1);
    pLayout->addLayout(pHLayout);
// снимаем фокус с pWidget
    this->setFocus();
}
void Content::setTextSize(const int &textSize)
{
    pTitle->setStyleSheet("font-size:" + QString::number(textSize + float(textSize) / 100 * 50 ) + "px;");
    home.setStyleSheet("font-size:" + QString::number(textSize + float(textSize) / 100 * 50 ) + "px;");
}
void Content::slotRestartTimer()
{
    timer.start(timerSec);
//    qDebug() << "restart timer";
}
bool Content::event(QEvent *event)
{
//    qDebug() << "Content" << event->type();
    if(event->type() == QEvent::Close)
        emit signalClose(); // сигнал используется в классе mini_widget

    return QWidget::event(event);
}
void Content::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

// ИСПРАВИТЬ!!! фон из настроек
    QPixmap newPix = backgoundImage->scaled(this->size(), Qt::IgnoreAspectRatio);
    painter.setBrush(QBrush(Qt::black, newPix));
    painter.drawRect(this->rect());
}
Content::~Content(){
//    delete pWidget;
    delete pHLayout;
    delete pLayout;
    delete pTitle;
    delete backgoundImage;
}
