#include "leafer.h"
#include "ui_leafer.h"

#include <QDebug>

Leafer::Leafer(int textSize, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Leafer)
{
    ui->setupUi(this);

    buttonSize = textSize*2;

    ui->count->setStyleSheet("font-size:" + QString::number(textSize) + "px;");
    ui->previos->setIconSize(QSize(buttonSize, buttonSize));
    ui->next->setIconSize(QSize(buttonSize, buttonSize));

    connect(ui->previos, SIGNAL(pressed()), this, SLOT(slotPreviosPush()));
    connect(ui->previos, SIGNAL(released()), this, SLOT(slotPreviosRelease()));

    connect(ui->next, SIGNAL(pressed()), this, SLOT(slotNextPush()));
    connect(ui->next, SIGNAL(released()), this, SLOT(slotNextRelease()));

}
void Leafer::setCounter(int currentPosition, int totalPages)
{
    QString result = "\0";
    result = QString::number(currentPosition) + " / " + QString::number(totalPages) ;
    ui->count->setText(result);
}

void Leafer::slotPreviosPush()
{
    ui->previos->setIconSize(QSize(buttonSize/2, buttonSize/2) );
}
void Leafer::slotNextPush()
{
    ui->next->setIconSize(QSize(buttonSize/2, buttonSize/2) );
}

void Leafer::slotPreviosRelease()
{
    ui->previos->setIconSize(QSize(buttonSize, buttonSize) );
    emit signalPrevios();
}
void Leafer::slotNextRelease()
{
    ui->next->setIconSize(QSize(buttonSize, buttonSize) );
    emit signalNext();
}

Leafer::~Leafer()
{
    delete ui;
}
