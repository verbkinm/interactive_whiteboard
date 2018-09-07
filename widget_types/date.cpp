#include "date.h"
#include <QDate>
#include <QTimer>

Date::Date(QString textColor, int textSize, QString backgroundColor, QString datePattern, QWidget *parent) : QLabel(parent)
{
    this->datePattern = datePattern;
    slotShowDate();
    this->setAlignment(Qt::AlignCenter);


    this->setStyleSheet("background-color: " + backgroundColor + ";color: " + textColor + "; \
                        font: bold " + QString::number(textSize) + "px;");

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotShowDate()));
    timer->start(60000);
}
void Date::slotShowDate()
{
    this->setText(QDate::currentDate().toString(datePattern));
}
