#include "clock.h"

#include <QTimer>
#include <QTime>
#include <QPainter>

#include <QDebug>

Clock::Clock(QString textColor, QString backgroundColor, QWidget *parent) : QLCDNumber(parent)
{
    this->setSegmentStyle(QLCDNumber::Flat);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotShowTime()));
    timer->start(200);
//чтобы при первом запуске не показывало 0
    display(QTime::currentTime().toString("hh:mm"));

    this->setStyleSheet("background-color: " + backgroundColor + ";color: " + textColor + ";");
}
void Clock::slotShowTime()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm");
    if ((time.second() % 2) == 0)
        text[2] = ' ';
    display(text);
}
