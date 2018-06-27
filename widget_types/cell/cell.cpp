#include "cell.h"

#include <QLabel>

#include <QDebug>
#include <QPainter>
#include <QBrush>
#include <QPaintEvent>
#include <QLinearGradient>

Cell::Cell(unsigned int textSize, QString textColor) : QWidget()
{
    this->textSize = textSize;
    this->textColor= textColor;

    pGridLayout = new QGridLayout;
//    QLabel* pl = new QLabel("label");
//    pHorizontLayout->addWidget(pl);
    this->setLayout(pGridLayout);

    setBackgroundColor(NONE);
}
void Cell::setText(QString str)
{
        QLabel* pLabel = new QLabel(str);
        pLabel->setStyleSheet("color:" + textColor + ";"
                              "font-size:" + QString::number(textSize) + "px;");
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
void Cell::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    switch (TYPE_COLUMN) {
    case NUMBER:{
//        QLinearGradient gradient(0, 0, width(), height());
//        gradient.setColorAt(0, QColor(255, 140, 0, 255));
//        gradient.setColorAt(1, Qt::white);
        painter.setBrush(QBrush(QColor(255, 140, 0, 255)));
        break;
        }
    case TIME:{
//        QLinearGradient gradient(0, 0, width(), height());
//        gradient.setColorAt(0, QColor(255, 228, 0, 255));
//        gradient.setColorAt(1, Qt::white);
//        painter.setBrush(gradient);
        painter.setBrush(QBrush(QColor(255, 228, 0, 255)));
        break;
        }
    case LESSON: {
//        QLinearGradient gradient(0, 0, width(), height());
//        gradient.setColorAt(0, QColor(0, 0, 255, 100));
//        gradient.setColorAt(1, Qt::white);
        painter.setBrush(QBrush(QColor(0, 0, 255, 50)));
        break;
        }
    default:
        painter.setBrush(QBrush(QColor(128, 128, 128, 20)));
        break;
    }
    painter.setPen(Qt::SolidLine);
    painter.drawRect(this->rect());
//    painter.drawRoundRect(this->rect(), 15, 15);
}
