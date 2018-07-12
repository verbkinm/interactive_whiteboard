#include "cell.h"

#include <QDebug>
#include <QPainter>
#include <QBrush>
#include <QPaintEvent>
#include <QLinearGradient>

Cell::Cell(unsigned int textSize, QString textColor, bool bold) : QWidget()
{
    this->textSize = textSize;
    this->textColor= textColor;
    this->bold     = bold;

    pGridLayout = new QGridLayout;
    this->setLayout(pGridLayout);

    setBackgroundColor(NONE);
}
void Cell::setText(QString str)
{
    QString boldText;

    if(bold)
        boldText = "font: bold;";

    pLabel = new QLabel(str);
    pLabel->setStyleSheet("color:" + textColor + ";"
            "font-size:" + QString::number(textSize) + "px;" +
            boldText );
    pLabel->setAlignment(Qt::AlignCenter);
    pGridLayout->addWidget(pLabel, row++, column);

    if (row > 2)
        row = 0;
}
void Cell::setBackgroundColor(tp role)
{
    TYPE_COLUMN = role;
}
void Cell::separate()
{
    row = 0;
    column++;
}
void Cell::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    switch (TYPE_COLUMN) {
    case NUMBER:{
        painter.setBrush(QBrush(QColor(255, 140, 0, 230)));
        break;
        }
    case TIME:{
        painter.setBrush(QBrush(QColor(255, 228, 0, 230)));
        break;
        }
    case LESSON: {

        painter.setBrush(QBrush(QColor(210, 210, 255, 230)));
        break;
        }
    case HEADER: {
        painter.setBrush(QBrush(QColor(203, 102, 51, 230)));
        break;
        }
    default:
        painter.setBrush(QBrush(QColor(200, 200, 200, 230)));
        break;
    }
    painter.setPen(Qt::SolidLine);
    painter.drawRect(this->rect());
}
Cell::~Cell()
{
//    qDebug() << "cell destructor";
    delete pLabel;
    delete pHorizontLayout;
    delete pGridLayout;
}
