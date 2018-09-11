#include "dontclick.h"

DontClick::DontClick(int textSize, QString textColor, QString backgroundColor, QWidget *parent) : QLabel(parent)
{
    this->setStyleSheet("background-color: " + backgroundColor + ";color: " + textColor + "; \
                        font: bold " + QString::number(textSize) + "px;");

    this->setAlignment(Qt::AlignCenter);
}
void DontClick::setCountPush(int count)
{
    this->setText("С момента запуска программы, кнопку нажали - " + QString::number(count) + " раз!");
}
