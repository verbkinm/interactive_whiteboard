#include "mini_widget.h"

#include <QDebug>
#include <QEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPropertyAnimation>


Mini_Widget::Mini_Widget(QString borderColor, int borderWidth, QString borderClickColor, int borderClickWidth, \
            QPixmap* miniIcon, QSize size, \
            QString xmlPath, QString dirPath,   \
            QString textColor, QString backgroundColor, \
            QString titleText, unsigned int textSize, QString type, QWidget *parent) : QWidget(parent)
{
    generalSettings(borderColor, borderWidth, borderClickColor, borderClickWidth, size);
    setTypeValue(type);

    switch (*(this->type)) {
        case LABEL:
                createLabelWidget(miniIcon, size);
            break;
        case CLOCK:
                createClockWidget(size, textColor, backgroundColor);
             break;
        case SCHEDULE:
                createScheduleWidget(miniIcon, size, xmlPath, textColor, textSize, titleText);
            break;
        case IMAGE_VIEWER:
                createImageViewerWidget(miniIcon, size, dirPath, textColor, titleText, textSize);
            break;

        default:
            break;
    }
}
void Mini_Widget::generalSettings(QString borderColor, int borderWidth, QString borderClickColor, int borderClickWidth, \
                                  QSize size)
{
    this->borderClickWidth  = borderClickWidth;
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
void Mini_Widget::createLabelWidget(QPixmap *miniIcon, QSize size)
{
    createLabelForMiniWidget(miniIcon, size);
}
void Mini_Widget::createClockWidget(QSize size, QString textColor, QString backgroudColor)
{
    centralWidget = new QWidget(this);
    centralWidget->move(borderClickWidth / 2, borderClickWidth/2);
    Clock* pClock = new Clock(textColor, backgroudColor, centralWidget);
    pClock->setFixedSize(size);
}
void Mini_Widget::createScheduleWidget(QPixmap *miniIcon, QSize size, \
                                       QString xmlPath, QString textColor, unsigned int textSize, QString titleText)
{
    this->xmlPath       = new QString(xmlPath);
    this->textColor     = new QString(textColor);
    this->textSize      = new unsigned int(textSize);

    createLabelForMiniWidget(miniIcon, size);

    pContent = new Content(titleText);
    pContent->setObjectName("Content");
    pContent->setTextSize(textSize);
}
void Mini_Widget::createImageViewerWidget(QPixmap *miniIcon, QSize size, \
                                          QString dirPath, QString textColor, \
                                          QString titleText, unsigned int textSize)
{
    this->dirPath       = new QString(dirPath);
    this->textColor     = new QString(textColor);
    this->textSize      = new unsigned int(textSize);
    this->titleText     = new QString(titleText);

    createLabelForMiniWidget(miniIcon, size);

    pContent = new Content(titleText);
    pContent->setObjectName("Content");
    pContent->setTextSize(textSize);
}
void Mini_Widget::setTypeValue(QString typeStr)
{
    if( typeStr == "label" )
        type = new unsigned int(LABEL);
    else if( typeStr == "clock" )
        type = new unsigned int(CLOCK);
    else if( typeStr == "schedule" )
        type = new unsigned int(SCHEDULE);
    else if( typeStr == "image_viewer" )
        type = new unsigned int(IMAGE_VIEWER);
}

void Mini_Widget::createLabelForMiniWidget(QPixmap* miniIcon, QSize size)
{
    centralLabel = new QLabel(this);
    centralLabel->setFixedSize(size);
    centralLabel->move(borderClickWidth / 2, borderClickWidth/2);

    QPixmap* newPixmap = new QPixmap;
    *newPixmap = miniIcon->scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    centralLabel->setPixmap(*newPixmap);
    centralLabel->setAlignment(Qt::AlignCenter);
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

            case SCHEDULE:{
                pSchedule = new Schedule(*xmlPath, *textColor, *textSize, pContent);
                pSchedule->setObjectName("Schedule");
                pContent->addWidget(pSchedule);
                pContent->setWindowModality(Qt::ApplicationModal);
                connect(pContent, SIGNAL(signalClose()), this, SLOT(slotDeleteWidgetInContent()));

                QPropertyAnimation* panim1 = new QPropertyAnimation(pContent, "geometry");
                panim1->setDuration(500);
                panim1->setStartValue( QRect(this->x(), this->y(), this->width(), this->height()) );
                panim1->setEndValue(QRect(static_cast<QWidget*>(this->parent())->rect()));
                panim1->setEasingCurve(QEasingCurve::Linear);

                panim1->start();
                pContent->repaint();
                break;
                }
            case IMAGE_VIEWER:{
                pImageViewer = new viewer(*dirPath, *textColor, *textSize, pContent);
                pImageViewer->setObjectName("ImageViewer");
                pContent->addWidget(pImageViewer);
                connect(pContent, SIGNAL(signalClose()), this, SLOT(slotDeleteWidgetInContent()));
                QPropertyAnimation* panim1 = new QPropertyAnimation(pContent, "geometry");
                panim1->setDuration(500);
                panim1->setStartValue( QRect(this->x(), this->y(), this->width(), this->height()) );
                panim1->setEndValue(QRect(static_cast<QWidget*>(this->parent())->rect()));
                panim1->setEasingCurve(QEasingCurve::InQuad);

                panim1->start();
                pContent->repaint();
                break;
                }
            default:
                break;
            }
            pContent->showFullScreen();
            pContent->setWindowModality(Qt::ApplicationModal);
        }
    }

    return QWidget::event(event);
}
void Mini_Widget::slotDeleteWidgetInContent()
{
    if(pSchedule != nullptr){
        delete pSchedule;
        disconnect(pContent, SIGNAL(signalClose()),this, SLOT(slotDeleteWidgetInContent()));
    }
    if(pImageViewer != nullptr){
        delete pImageViewer;
        disconnect(pContent, SIGNAL(signalClose()),this, SLOT(slotDeleteWidgetInContent()));
    }

}
