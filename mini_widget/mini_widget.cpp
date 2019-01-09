#include "mini_widget.h"

#include <QDebug>
#include <QEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QTextCodec>

//задержка анимациив
#define DURATION 1000

Mini_Widget::Mini_Widget(settingsMiniWidget *struct_settingsMiniWidget, \
                         QWidget *parent) : QWidget(parent)
{
    borderClick->setLayout(&borderClickLayout);

    mainStruct_settingsMiniWidget = *struct_settingsMiniWidget;

    generalSettings();

    switch (mainStruct_settingsMiniWidget.pGetType(mainStruct_settingsMiniWidget.miscellanea.type)) {
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
        case DONT_CLICK:
                createDontClickWidget();
            break;
        case BELLS_MONITOR:
                createBellsMonitor();
            break;

        //bells

        default:
            break;
    }
}
void Mini_Widget::generalSettings()
{
//рамка, которая будет появлятся при нажатии
    borderClick->setStyleSheet("background-color: #00000000; border-radius: 20px;");
    borderClick->setFixedSize(mainStruct_settingsMiniWidget.size);
    borderClick->layout()->setMargin(mainStruct_settingsMiniWidget.border.borderClickWidth);
//рамка
    borderClickLayout.addWidget(&border);
    border.setLayout(&borderLayout);

    border.layout()->setMargin(mainStruct_settingsMiniWidget.border.borderWidth);
    //example: border-color: rgba(255, 0, 0, 75%)
    border.setStyleSheet("background-color:" + mainStruct_settingsMiniWidget.border.borderColor + ";");
}
void Mini_Widget::createLabelWidget()
{
    centralWidgetForMiniWidget = new WidgetForMiniWidget(&mainStruct_settingsMiniWidget, &border);
    borderLayout.addWidget(centralWidgetForMiniWidget);

    connect(centralWidgetForMiniWidget, SIGNAL(signalImagePressed()), this, SLOT(slotWidgetPressed()));
    connect(centralWidgetForMiniWidget, SIGNAL(signalImageReleased()), this, SLOT(slotWidgetReleased()));
    connect(centralWidgetForMiniWidget, SIGNAL(signalImageClicked()), this, SLOT(slotWidgetClicked()));
}
void Mini_Widget::createClockWidget()
{
    centralWidgetForMiniWidget = new WidgetForMiniWidget(&mainStruct_settingsMiniWidget, &border);
    borderLayout.addWidget(centralWidgetForMiniWidget);

    pClock = new Clock(mainStruct_settingsMiniWidget.text.textColor, \
                       mainStruct_settingsMiniWidget.background.backgroundColor, \
                       centralWidgetForMiniWidget);

    pClock->setObjectName("clock");

    centralWidgetForMiniWidget->addMainWidget(pClock);
}
void Mini_Widget::createDateWidget()
{
    centralWidgetForMiniWidget = new WidgetForMiniWidget(&mainStruct_settingsMiniWidget, &border);
    borderLayout.addWidget(centralWidgetForMiniWidget);

    pDate = new Date(mainStruct_settingsMiniWidget.text.textColor, \
                     mainStruct_settingsMiniWidget.text.textSize, \
                     mainStruct_settingsMiniWidget.background.backgroundColor, \
                     mainStruct_settingsMiniWidget.miscellanea.datePattern, \
                     centralWidgetForMiniWidget);
    pDate->setObjectName("date");

    centralWidgetForMiniWidget->addMainWidget(pDate);
}
void Mini_Widget::createRunStringWidget()
{
    QString text = txtFileToString(mainStruct_settingsMiniWidget.path.txtPath);

    centralWidgetForMiniWidget = new WidgetForMiniWidget(&mainStruct_settingsMiniWidget, &border);
    borderLayout.addWidget(centralWidgetForMiniWidget);

    pRun_String = new Run_String(mainStruct_settingsMiniWidget.text.textColor, \
                                 mainStruct_settingsMiniWidget.text.textSize, \
                                 mainStruct_settingsMiniWidget.background.backgroundColor, \
                                 text, \
                                 mainStruct_settingsMiniWidget.miscellanea.speed, \
                                 centralWidgetForMiniWidget);
    pRun_String->setObjectName("run_string");

    centralWidgetForMiniWidget->addMainWidget(pRun_String);
}
void Mini_Widget::createScheduleWidget()
{
    createLabelWidget();

    pContent = new Content(&mainStruct_settingsMiniWidget);
    pContent->setObjectName("Content");
    pContent->setTextSize(mainStruct_settingsMiniWidget.text.textSize);
}
void Mini_Widget::createImageViewerWidget()
{
    createLabelWidget();

    pContent = new Content(&mainStruct_settingsMiniWidget);
    pContent->setObjectName("Content");
    pContent->setTextSize(mainStruct_settingsMiniWidget.text.textSize);
}
void Mini_Widget::createDontClickWidget()
{
    createLabelWidget();

    pContent = new Content(&mainStruct_settingsMiniWidget);
    pContent->setObjectName("Content");
    pContent->setTextSize(mainStruct_settingsMiniWidget.text.textSize);
}
void Mini_Widget::createBellsMonitor()
{
    centralWidgetForMiniWidget = new WidgetForMiniWidget(&mainStruct_settingsMiniWidget, &border);
    borderLayout.addWidget(centralWidgetForMiniWidget);

    pBellsMonitor = new BellsMonitor(centralWidgetForMiniWidget);

    pBellsMonitor->setObjectName("bels_monitor");

    centralWidgetForMiniWidget->addMainWidget(pBellsMonitor);
}
void Mini_Widget::slotWidgetPressed()
{
    borderClick->setStyleSheet("background-color:" + mainStruct_settingsMiniWidget.border.borderClickColor + "; border-radius: 20px;");
}
void Mini_Widget::slotWidgetReleased()
{
    borderClick->setStyleSheet("background-color: #00000000; border-radius: 20px;");
}
void Mini_Widget::slotWidgetClicked()
{
    slotWidgetReleased();
    if(pContent != nullptr){
        switch (mainStruct_settingsMiniWidget.pGetType(mainStruct_settingsMiniWidget.miscellanea.type)) {
            case LABEL:

                break;
            case CLOCK:
                break;
            case DATE:
                break;
            case RUN_STRING:
                break;

            case SCHEDULE:
                pSchedule = new Schedule(mainStruct_settingsMiniWidget.path.xmlPath, \
                                         mainStruct_settingsMiniWidget.text.textColor, \
                                         mainStruct_settingsMiniWidget.text.textSize,
                                         pContent);
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
                    delete pSchedule;
                    pSchedule = nullptr;
                    pContent->slotAnimCloseWindow();
                }

                break;

            case IMAGE_VIEWER:
                pImageViewer = new viewer(mainStruct_settingsMiniWidget.path.dirPath, \
                                          mainStruct_settingsMiniWidget.text.textColor, \
                                          mainStruct_settingsMiniWidget.text.textSize, \
                                          centralWidgetForMiniWidget->getCurrentPage(), \
                                          pContent);
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
                    delete pImageViewer;
                    pImageViewer = nullptr;
                    pContent->slotAnimCloseWindow();
                }

                break;

            case DONT_CLICK:
                static int countPush = 0;

                pDontClick = new DontClick(mainStruct_settingsMiniWidget.text.textSize, \
                                           mainStruct_settingsMiniWidget.text.textColor, \
                                           mainStruct_settingsMiniWidget.background.backgroundColor, \
                                           pContent);
                pDontClick->setCountPush(++countPush);
                pDontClick->setObjectName("DontClick");
                pContent->addWidget(pDontClick);
                connect(pContent, SIGNAL(signalClose()), this, SLOT(slotDeleteWidgetInContent()));

                panimOpen = new QPropertyAnimation(pContent, "geometry");
                panimOpen->setDuration(DURATION);
                panimOpen->setStartValue( QRect(this->x(), this->y(), this->width(), this->height()) );
                panimOpen->setEndValue(QRect(static_cast<QWidget*>(this->parent())->rect()));
                panimOpen->setEasingCurve(QEasingCurve::CosineCurve);
                panimOpen->start();

                break;

            case BELLS_MONITOR:
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
        delete pSchedule;
        pSchedule = nullptr;
        disconnect(pContent, SIGNAL(signalClose()),this, SLOT(slotDeleteWidgetInContent()));
    }
    if(pImageViewer != nullptr){
        disconnect(pImageViewer, SIGNAL(signalTimerStart()), pContent, SLOT(slotRestartTimer()));
        delete pImageViewer;
        pImageViewer = nullptr;
        disconnect(pContent, SIGNAL(signalClose()),this, SLOT(slotDeleteWidgetInContent()));
    }
    if(pDontClick != nullptr){
        delete pDontClick;
        pDontClick = nullptr;
        disconnect(pContent, SIGNAL(signalClose()),this, SLOT(slotDeleteWidgetInContent()));
    }

    if(panimOpen != nullptr)
        delete panimOpen;

}
QString Mini_Widget::txtFileToString(QString filePath)
{
    QString text;
    QFile file(filePath);

    if( !(file.open(QIODevice::ReadOnly)) || file.size() > 1000 * 1024)
        text = "ОШИБКА ОТКРЫТИЯ ФАЙЛА \"" + filePath + "\"!";
    else
        text = QString::fromUtf8(file.readAll());

    file.close();

    return text;
}
void Mini_Widget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
Mini_Widget::~Mini_Widget()
{
    if(pSchedule != nullptr)
        delete pSchedule;

    if(pImageViewer != nullptr)
        delete pImageViewer;
    if(pDontClick != nullptr)
        delete pDontClick;

//    if(type != nullptr)
//        delete type;


    if(pContent != nullptr)
        delete pContent;

//    if(centralLabel != nullptr)
//        delete centralLabel;

//    if(borderClick != nullptr)
//        delete borderClick;

//    if(border != nullptr)
//        delete border;

//    if(centralWidget != nullptr)
//        delete centralWidget;
}
