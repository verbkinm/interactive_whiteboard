#include<QPainter>
#include <QEvent>
#include <QDebug>
#include <QHBoxLayout>
#include <QTimer>

#include "content.h"

Content::Content(const struct text &struct_text, QString backgoundImagePath, int timerSec, QWidget *parent) : QLabel(parent)
{
    timerSec *= 1000;
    this->struct_text = struct_text;
    this->struct_text.textSize = this->struct_text.textSize + float(this->struct_text.textSize) / 100 * 50;

    this->backgoundImage = new QPixmap(backgoundImagePath);

    pLayout = new QVBoxLayout();
    this->setLayout(pLayout);

    pTitle = new QLabel(this->struct_text.titleText);
    pTitle->setAlignment(Qt::AlignCenter);
    pLayout->addWidget(pTitle);


    home.setText("Закрыть");
    connect(&home, SIGNAL(clicked(bool)), this, SLOT(slotAnimCloseWindow()));

//таймер бездействия
    connect(&timer, SIGNAL(timeout()), this, SLOT(slotAnimCloseWindow()));
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
//    this->setFocus();
}
void Content::setTextSize(const int &textSize)
{
    pTitle->setStyleSheet("font-size:" + QString::number(textSize + float(textSize) / 100 * 50 ) + "px;");
    home.setStyleSheet("font-size:" + QString::number(textSize + float(textSize) / 100 * 50 ) + "px;");
}
void Content::setTitle(const QString &title)
{
    pTitle->setText(struct_text.titleText + title);
    pTitle->setStyleSheet("font: bold " + QString::number(struct_text.textSize) + "px;");
}
void Content::slotAnimCloseWindow()
{
    panimClose = new QPropertyAnimation(this, "windowOpacity");
    panimClose->setDuration(1000);
    panimClose->setStartValue(1);
    panimClose->setEndValue(0);
    panimClose->start();

//при закрытии окна, оно на мгновение появляеться полностью, чтобы это избежать - с начало окно скрываем, потом закрываем
    connect(panimClose, SIGNAL(finished()), this, SLOT(hide()));
    connect(panimClose, SIGNAL(finished()), this, SLOT(close()));
}
void Content::slotRestartTimer()
{
    timer.start(timerSec);
//    qDebug() << "restart timer";
}
bool Content::event(QEvent *event)
{
//    qDebug() << "Content" << event->type();
    if(event->type() == QEvent::Close){
        this->setWindowOpacity(1);
        delete panimClose;
        emit signalClose(); // сигнал используется в классе mini_widget
    }

    return QWidget::event(event);
}
void Content::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

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
