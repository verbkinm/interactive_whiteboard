#include "mini_widget.h"

#include <QDebug>
#include <QEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QTextCodec>

//задержка анимациив
#define DURATION 1000

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
        case DATE:
                createDateWidget();
             break;
        case RUN_STRING:
                createRunStringWidget();
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
    QWidget *centralWidget = new QWidget(this);
    centralWidget->move(struct_border.borderClickWidth / 2, struct_border.borderClickWidth/2);
    Clock* pClock = new Clock(struct_text.textColor, struct_background.backgroundColor, centralWidget);
    pClock->setFixedSize(_size);
}
void Mini_Widget::createDateWidget()
{
    QWidget *centralWidget = new QWidget(this);
    centralWidget->move(struct_border.borderClickWidth / 2, struct_border.borderClickWidth/2);
    Date*  pDate = new Date(struct_text.textColor, struct_text.textSize, \
                            struct_background.backgroundColor, struct_miscellanea.datePattern, \
                            centralWidget);
    pDate->setFixedSize(_size);
}
void Mini_Widget::createRunStringWidget()
{
    QString text;
    QFile file(struct_path.txtPath);

    if( !(file.open(QIODevice::ReadOnly)) || file.size() > 10 * 1024)
        text = "ОШИБКА ОТКРЫТИЯ ФАЙЛА \"" + struct_path.txtPath + "\"!";
    else
        text = QString::fromUtf8(file.readAll());

    file.close();

    QWidget *centralWidget = new QWidget(this);
    centralWidget->move(struct_border.borderClickWidth / 2, struct_border.borderClickWidth/2);
    Run_String*  pRun_String = new Run_String(struct_text.textColor, struct_text.textSize, \
                            struct_background.backgroundColor, text, struct_miscellanea.speed, \
                            centralWidget);
    pRun_String->setFixedSize(_size);
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

    pContent = new Content(struct_text, struct_background.backgroundImage, struct_miscellanea.timerSec);
    pContent->setObjectName("Content");
    pContent->setTextSize(struct_text.textSize);
}
void Mini_Widget::setTypeValue(QString typeStr)
{
    if( typeStr == "label" )
        type = new int(LABEL);
    else if( typeStr == "clock" )
        type = new int(CLOCK);
    else if( typeStr == "date" )
        type = new int(DATE);
    else if( typeStr == "run_string" )
        type = new int(RUN_STRING);
    else if( typeStr == "schedule" )
        type = new int(SCHEDULE);
    else if( typeStr == "image_viewer" )
        type = new int(IMAGE_VIEWER);
}

void Mini_Widget::createLabelForMiniWidget()
{
   centralWidgetForMiniWidget = new WidgetForMiniWidget(&struct_path, \
                                                        &struct_text, \
                                                        &struct_miscellanea, \
                                                        &_size, \
                                                        this);
    centralWidgetForMiniWidget->move(struct_border.borderClickWidth / 2, \
                        struct_border.borderClickWidth / 2);

    connect(centralWidgetForMiniWidget, SIGNAL(signalImagePressed()), this, SLOT(slotWidgetPressed()));
    connect(centralWidgetForMiniWidget, SIGNAL(signalImageReleased()), this, SLOT(slotWidgetReleased()));
    connect(centralWidgetForMiniWidget, SIGNAL(signalImageClicked()), this, SLOT(slotWidgetClicked()));
}
void Mini_Widget::slotWidgetPressed()
{
    borderClick->setVisible(true);
}
void Mini_Widget::slotWidgetReleased()
{
    borderClick->setVisible(false);
}
void Mini_Widget::slotWidgetClicked()
{
    slotWidgetReleased();
    if(pContent != 0 && type != 0){
        switch (*type) {
            case LABEL:

                break;
            case CLOCK:
                break;
            case DATE:
                break;
            case RUN_STRING:
                break;

            case SCHEDULE:{
                pSchedule = new Schedule(struct_path.xmlPath, struct_text.textColor, struct_text.textSize, pContent);
                if(!pSchedule->CRITICAL_ERROR)
                {
                    pSchedule->setObjectName("Schedule");
                    connect(pSchedule, SIGNAL(signalTimerStart()), pContent, SLOT(slotRestartTimer()));
                    pContent->addWidget(pSchedule);
                    pContent->setTitle(pSchedule->getTitle());
                    connect(pContent, SIGNAL(signalClose()), this, SLOT(slotDeleteWidgetInContent()));

                    panimOpen = new QPropertyAnimation(pContent, "geometry");
                    panimOpen->setDuration(DURATION);
                    panimOpen->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
                    panimOpen->setEndValue(QRect(static_cast<QWidget*>(this->parent())->rect()));
                    panimOpen->setEasingCurve(QEasingCurve::Linear);
                    panimOpen->start();
                }
                else
                {
                    pSchedule->~Schedule();
                    pSchedule = nullptr;
                    pContent->slotAnimCloseWindow();
                }

            }
                break;

            case IMAGE_VIEWER:{
                pImageViewer = new viewer(struct_path.dirPath, struct_text.textColor, struct_text.textSize, \
                                          centralWidgetForMiniWidget->getCurrentPage(), pContent);
                if(!pImageViewer->CRITICAL_ERROR)
                {
                    pImageViewer->setObjectName("ImageViewer");
                    connect(pImageViewer, SIGNAL(signalTimerStart()), pContent, SLOT(slotRestartTimer()));
                    pContent->addWidget(pImageViewer);
                    connect(pContent, SIGNAL(signalClose()), this, SLOT(slotDeleteWidgetInContent()));

                    panimOpen = new QPropertyAnimation(pContent, "geometry");
                    panimOpen->setDuration(DURATION);
                    panimOpen->setStartValue( QRect(this->x(), this->y(), this->width(), this->height()) );
                    panimOpen->setEndValue(QRect(static_cast<QWidget*>(this->parent())->rect()));
                    panimOpen->setEasingCurve(QEasingCurve::InQuad);
                    panimOpen->start();
                }
                else
                {
                    pImageViewer->~viewer();
                    pImageViewer = nullptr;
                    pContent->slotAnimCloseWindow();
                }
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
bool Mini_Widget::event(QEvent *event)
{
//    qDebug() << event->type();

    return QWidget::event(event);
}
void Mini_Widget::slotDeleteWidgetInContent()
{
    if(pSchedule != nullptr){
        disconnect(pSchedule, SIGNAL(signalTimerStart()), pContent, SLOT(slotRestartTimer()));
        pSchedule->~Schedule();
        pSchedule = nullptr;
        disconnect(pContent, SIGNAL(signalClose()),this, SLOT(slotDeleteWidgetInContent()));
    }
    if(pImageViewer != nullptr){
        disconnect(pImageViewer, SIGNAL(signalTimerStart()), pContent, SLOT(slotRestartTimer()));
        pImageViewer->~viewer();
        pImageViewer = nullptr;
        disconnect(pContent, SIGNAL(signalClose()),this, SLOT(slotDeleteWidgetInContent()));
    }
    delete panimOpen;

}
Mini_Widget::~Mini_Widget()
{
    if(pSchedule != nullptr)
        delete pSchedule;

    if(pImageViewer != nullptr)
        delete pImageViewer;

    if(type != nullptr)
        delete type;


    if(pContent != nullptr)
        delete pContent;

    if(centralLabel != nullptr)
        delete centralLabel;

    if(borderClick != nullptr)
        delete borderClick;

    if(border != nullptr)
        delete border;
}
