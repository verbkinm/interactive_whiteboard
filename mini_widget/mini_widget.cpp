#include "mini_widget.h"

#include <QDebug>
#include <QEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPropertyAnimation>

Mini_Widget::Mini_Widget(const struct border &struct_border, QSize size, \
                         const struct path &struct_path, \
                         const struct background &struct_background, \
                         const struct text &struct_text, \
                         const struct miscellanea &struct_miscellanea, \
                         QWidget *parent) : QWidget(parent)
{
    this->struct_border     = struct_border;
    this->struct_text       = struct_text;
    this->struct_path       = struct_path;
    this->struct_background = struct_background;
    this->struct_miscellanea= struct_miscellanea;

    this->_size             = size;

    generalSettings();
    setTypeValue(this->struct_miscellanea.type);

    switch (*(this->type)) {
        case LABEL:
                createLabelWidget();
            break;
        case CLOCK:
                createClockWidget();
             break;
        case SCHEDULE:
                createScheduleWidget();
            break;
        case IMAGE_VIEWER:
                createImageViewerWidget();
            break;

        default:
            break;
    }
}
void Mini_Widget::generalSettings()
{
//рамка, которая будет появлятся при нажатии
    borderClick              = new QLabel(this);
    //example: border-color: rgba(255, 0, 0, 75%)
    borderClick->setStyleSheet("background-color:" + struct_border.borderClickColor + ";");
    borderClick->setFixedSize(_size.width() + struct_border.borderClickWidth, _size.height() + struct_border.borderClickWidth);
    borderClick->hide();

//рамка
    border              = new QLabel(this);
    //example: border-color: rgba(255, 0, 0, 75%)
    border->setStyleSheet("background-color:" + struct_border.borderColor + ";");
    border->setFixedSize(_size.width() + struct_border.borderWidth, _size.height() + struct_border.borderWidth);
    border->move(struct_border.borderClickWidth / 2 - struct_border.borderWidth / 2, struct_border.borderClickWidth / 2 - struct_border.borderWidth / 2);

    this->setStyleSheet("border-radius: 20px;");
    this->setFixedSize(borderClick->size());
}
void Mini_Widget::createLabelWidget()
{
    createLabelForMiniWidget();
}
void Mini_Widget::createClockWidget()
{
    centralWidget = new QWidget(this);
    centralWidget->move(struct_border.borderClickWidth / 2, struct_border.borderClickWidth/2);
    pClock = new Clock(struct_text.textColor, struct_background.backgroundColor, centralWidget);
    pClock->setFixedSize(_size);
}
void Mini_Widget::createScheduleWidget()
{
    createLabelForMiniWidget();

    pContent = new Content(struct_text, struct_background.backgroundImage, struct_miscellanea.timerSec);
    pContent->setObjectName("Content");
    pContent->setTextSize(struct_text.textSize);
}
void Mini_Widget::createImageViewerWidget()
{
    createLabelForMiniWidget();

    pContent = new Content(struct_text, struct_background.backgroundImage);
    pContent->setObjectName("Content");
    pContent->setTextSize(struct_text.textSize);
}
void Mini_Widget::setTypeValue(QString typeStr)
{
    if( typeStr == "label" )
        type = new int(LABEL);
    else if( typeStr == "clock" )
        type = new int(CLOCK);
    else if( typeStr == "schedule" )
        type = new int(SCHEDULE);
    else if( typeStr == "image_viewer" )
        type = new int(IMAGE_VIEWER);
}

void Mini_Widget::createLabelForMiniWidget()
{
    centralLabel = new QLabel(this);
    centralLabel->setFixedSize(_size);
    centralLabel->move(struct_border.borderClickWidth / 2, struct_border.borderClickWidth / 2);

    QPixmap pixMap(struct_path.iconPath);
    QPixmap newPixmap = pixMap.scaled(_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    centralLabel->setPixmap(newPixmap);
    centralLabel->setAlignment(Qt::AlignCenter);
}
bool Mini_Widget::event(QEvent *event)
{
//    qDebug() << event->type();
    static bool cursorOnWidget;
    static bool pressingButton;

// при нажатии на виджет появляется рамка, при ударживании кнопки нажатия и вывыедения курсора за границы виджета рамка исчезает,
// а при возвращении обратно - рамка появляется
    if(event->type() == QEvent::MouseMove){
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->x() < 0 || mouseEvent->y() < 0 || mouseEvent->x() > this->width() || mouseEvent->y() > this->height()){
            this->borderClick->setVisible(false);
            cursorOnWidget = false;
        }
        else if(mouseEvent->x() > 0 && mouseEvent->y() > 0 && mouseEvent->x() < this->width() && mouseEvent->y() < this->height()){
            this->borderClick->setVisible(true);
            cursorOnWidget = true;
        }
    }
    if(event->type() == QEvent::MouseButtonPress){
        borderClick->setVisible(true);
        cursorOnWidget = true;
        pressingButton = true;
    }
// при отпускании кнопки проверяется, где в данный момент находится курсор - над виджетом или нет.
// Если над виджетом - выполняются последующие действия
    if(event->type() == QEvent::MouseButtonRelease){
        borderClick->setVisible(false);
        pressingButton = false;
        if(pContent != 0 && type != 0 && cursorOnWidget){
            switch (*type) {
                case LABEL:

                    break;
                case CLOCK:
                    break;

                case SCHEDULE:{
                    pSchedule = new Schedule(struct_path.xmlPath, struct_text.textColor, struct_text.textSize, pContent);
                    pSchedule->setObjectName("Schedule");
                    connect(pSchedule, SIGNAL(signalTimerStart()), pContent, SLOT(slotRestartTimer()));
                    pContent->addWidget(pSchedule);
                    pContent->setTitle(pSchedule->getTitle());
                    connect(pContent, SIGNAL(signalClose()), this, SLOT(slotDeleteWidgetInContent()));

                    panimOpen = new QPropertyAnimation(pContent, "geometry");
                    panimOpen->setDuration(500);
                    panimOpen->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
                    panimOpen->setEndValue(QRect(static_cast<QWidget*>(this->parent())->rect()));
                    panimOpen->setEasingCurve(QEasingCurve::Linear);
                    panimOpen->start();
                }
                    break;

                case IMAGE_VIEWER:{
                    pImageViewer = new viewer(struct_path.dirPath, struct_text.textColor, struct_text.textSize, pContent);
                    pImageViewer->setObjectName("ImageViewer");
                    connect(pImageViewer, SIGNAL(signalTimerStart()), pContent, SLOT(slotRestartTimer()));
                    pContent->addWidget(pImageViewer);
                    connect(pContent, SIGNAL(signalClose()), this, SLOT(slotDeleteWidgetInContent()));

                    panimOpen = new QPropertyAnimation(pContent, "geometry");
                    panimOpen->setDuration(500);
                    panimOpen->setStartValue( QRect(this->x(), this->y(), this->width(), this->height()) );
                    panimOpen->setEndValue(QRect(static_cast<QWidget*>(this->parent())->rect()));
                    panimOpen->setEasingCurve(QEasingCurve::InQuad);
                    panimOpen->start();
                }
                    break;

                default:
                    break;
            }
            pContent->setWindowFlag(Qt::SplashScreen);
            pContent->setWindowModality(Qt::ApplicationModal);
            pContent->show();
        }
    }

    return QWidget::event(event);
}
void Mini_Widget::slotDeleteWidgetInContent()
{
    if(pSchedule != nullptr){
        disconnect(pSchedule, SIGNAL(signalTimerStart()), pContent, SLOT(slotRestartTimer()));
        delete pSchedule;
        delete panimOpen;
        pSchedule = nullptr;
        disconnect(pContent, SIGNAL(signalClose()),this, SLOT(slotDeleteWidgetInContent()));
    }
    if(pImageViewer != nullptr){
        disconnect(pImageViewer, SIGNAL(signalTimerStart()), pContent, SLOT(slotRestartTimer()));
        delete pImageViewer;
        delete panimOpen;
        pImageViewer = nullptr;
        disconnect(pContent, SIGNAL(signalClose()),this, SLOT(slotDeleteWidgetInContent()));
    }

}
Mini_Widget::~Mini_Widget()
{
    if(pSchedule != nullptr)
        delete pSchedule;

    if(pImageViewer != nullptr)
        delete pImageViewer;

    if(type != nullptr)
        delete type;

    if(pClock != nullptr)
        delete pClock;

    if(centralWidget != nullptr)
        delete centralWidget;

    if(pContent != nullptr)
        delete pContent;

    if(centralLabel != nullptr)
        delete centralLabel;

    if(borderClick != nullptr)
        delete borderClick;

    if(border != nullptr)
        delete border;
}
