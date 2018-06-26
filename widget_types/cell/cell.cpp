#include "cell.h"

#include <QLabel>

#include <QDebug>

Cell::Cell(unsigned int textSize, QString textColor) : QWidget()
{
    this->textSize = textSize;
    this->textColor= textColor;

    pGridLayout = new QGridLayout;
//    QLabel* pl = new QLabel("label");
//    pHorizontLayout->addWidget(pl);
    this->setLayout(pGridLayout);
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
void Cell::separate()
{
    row = 0;
    column++;
}
