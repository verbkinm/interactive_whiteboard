#include "mini_widget.h"

#include <QDebug>
#include <QEvent>
#include <QPaintEvent>
#include <QPainter>

//label
Mini_Widget::Mini_Widget(QString borderColor, int borderWidth, QString borderClickColor, int borderClickWidth, \
                         QPixmap* miniIcon, QSize size, \
                         QWidget *parent) : QWidget(parent)
{
    generalSettings(borderColor, borderWidth, borderClickColor, borderClickWidth, size);
    createLabelForMiniWidget(borderClickWidth, miniIcon, size);
}
//type clock
Mini_Widget::Mini_Widget(QString borderColor, int borderWidth, QString borderClickColor, int borderClickWidth, \
                         QSize size, QString textColor, QString backgroudColor, QWidget *parent) : QWidget(parent)
{
    generalSettings(borderColor, borderWidth, borderClickColor, borderClickWidth, size);
    centralWidget = new QWidget(this);
    centralWidget->move(borderClickWidth / 2, borderClickWidth/2);
    Clock* pClock = new Clock(textColor, backgroudColor, centralWidget);
    pClock->setFixedSize(size);
}
//type schedule
Mini_Widget::Mini_Widget(QString borderColor, int borderWidth, QString borderClickColor, int borderClickWidth, \
            QPixmap* miniIcon, QSize size, \
            QString xmlPath,   QString textColor, unsigned int textSize, QWidget *parent) : QWidget(parent)
{
    this->xmlPath       = new QString(xmlPath);
    this->textColor     = new QString(textColor);
    this->textSize      = new unsigned int(textSize);
    this->type          = new unsigned int(SCHEDULE);

    generalSettings(borderColor, borderWidth, borderClickColor, borderClickWidth, size);
    createLabelForMiniWidget(borderClickWidth, miniIcon, size);

    pContent = new Content;
    pContent->setObjectName("Content");
    pContent->setTextSize(textSize);
}
void Mini_Widget::generalSettings(QString borderColor, int borderWidth, QString borderClickColor, int borderClickWidth, QSize size)
{
//рамка, которая будет появлятся при нажатии
    borderClick              = new QLabel(this);
    //example: border-color: rgba(255, 0, 0, 75%)
    borderClick->setStyleSheet("background-color:" + borderClickColor + ";");
    borderClick->setFixedSize(size.width() + borderClickWidth, size.height() + borderClickWidth);
    borderClick->hide();

//рамка
    border              = new QLabel(this);
    //example: border-color: rgba(255, 0, 0, 75%)
    border->setStyleSheet("background-color:" + borderColor + ";");
    border->setFixedSize(size.width() + borderWidth, size.height() + borderWidth);
    border->move(borderClickWidth / 2 - borderWidth / 2, borderClickWidth / 2 - borderWidth / 2);

    this->setStyleSheet("border-radius: 20px;");
    this->setFixedSize(borderClick->size());
}
void Mini_Widget::createLabelForMiniWidget(int borderClickWidth, QPixmap* miniIcon, QSize size)
{
    centralLabel = new QLabel(this);
    centralLabel->setFixedSize(size);
    centralLabel->move(borderClickWidth / 2, borderClickWidth/2);

    QPixmap* newPixmap = new QPixmap;
    *newPixmap = miniIcon->scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    centralLabel->setPixmap(*newPixmap);
}
void Mini_Widget::paintEvent(QPaintEvent *event)
{

}
bool Mini_Widget::event(QEvent *event)
{
//    qDebug() << event->type();
    if(event->type() == QEvent::MouseButtonPress){
        borderClick->setVisible(true);
    }
    if(event->type() == QEvent::MouseButtonRelease){
        borderClick->setVisible(false);
        if(pContent != 0 && type != 0){
            switch (*type) {
            case LABEL:

                break;
            case CLOCK:
                break;
            case SCHEDULE:
                pSchedule = new Schedule(*xmlPath, *textColor, *textSize, pContent);
                pSchedule->setObjectName("Schedule");
                pContent->addWidget(pSchedule);
                connect(pContent, SIGNAL(signalClose()), this, SLOT(slotDeletepSchedule()));
                break;

//            default:
//                break;
            }
            pContent->showFullScreen();
            pContent->setWindowModality(Qt::ApplicationModal);
        }
    }

    return QWidget::event(event);
}
void Mini_Widget::slotDeletepSchedule()
{
    delete pSchedule;
    disconnect(pContent, SIGNAL(signalClose()),this, SLOT(slotDeletepSchedule()));
}
