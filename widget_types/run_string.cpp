#include "run_string.h"

#include <QPainter>

Run_String::Run_String(QString textColor, int textSize, QString backgroundColor, QString text, int speed, QWidget *parent) : QLabel(parent)
{
    x = this->width();

    connect(&timer, SIGNAL(timeout()), this, SLOT(slotRunLine()));
    timer.start(speed);

    str = text;

    font = new QFont("Arial", textSize, QFont::Bold);
    fontMetric = new QFontMetrics(*font);

    stringLenght = fontMetric->width(str);

    this->setStyleSheet("background-color: " + backgroundColor + ";color: " + textColor + ";");
}
void Run_String::slotRunLine()
{
    x -= 1;

    repaint();
}
void Run_String::paintEvent(QPaintEvent *event)
{
    x_end = x + fontMetric->width(str);;

    if(x_end == 0)
        x = this->width();

    QPainter painter(this);

    painter.setFont(*font);

    int width;
    if(stringLenght < this->width())
        width = this->width();
    else
        width = stringLenght;

    painter.drawText(x,0, width, this->height(), Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextExpandTabs, str);
}
Run_String::~Run_String()
{
    delete fontMetric;
    delete font;
}
