#ifndef MINI_WIDGET_H
#define MINI_WIDGET_H

#include "widget_types/schedule.h"
#include "widget_types/myWidgets/viewer/viewer.h"
#include "widget_types/clock.h"
#include "widget_types/date.h"
#include "widget_types/run_string.h"
#include "widget_types/myWidgets/easter_bunny/dontclick.h"
#include "widget_types/bells_monitor/bellsmonitor.h"
#include "widgetforminiwidget.h"


#include "content/content.h"
#include "structes/structes.h"

#include <QWidget>
#include <QLabel>
#include <QSize>
#include <QPropertyAnimation>


class Mini_Widget : public QWidget
{
    Q_OBJECT
public:

    Mini_Widget(struct settingsMiniWidget *struct_settingsMiniWidget, \
                QWidget *parent = nullptr);

    ~Mini_Widget();

private:
    // функции для создания виджетов по их типу
    void createLabelWidget();
    void createClockWidget();
    void createDateWidget();
    void createRunStringWidget();
    void createScheduleWidget();
    void createImageViewerWidget();
    void createDontClickWidget();
    void createBellsMonitor();

    QString txtFileToString(QString filePath);

    WidgetForMiniWidget *centralWidgetForMiniWidget = nullptr;

//рамка
    QWidget                border;
//рамка, которая будет появлятся при нажатии
    Mini_Widget*            borderClick     = this;

// структура настроек для мини виджета
    settingsMiniWidget mainStruct_settingsMiniWidget;

    QVBoxLayout            borderClickLayout;
    QVBoxLayout            borderLayout;

// указатель для анимаций
    QPropertyAnimation* panimOpen           = nullptr;

//указатель на содержимое мини виджета, всё что открывается - находится в контейнере класса Content
    Content*                pContent        = nullptr;

//указатели на типы содержиого мини виджета
    Clock*                  pClock          = nullptr;
    Date*                   pDate           = nullptr;
    Run_String*             pRun_String     = nullptr;
    Schedule                *pSchedule      = nullptr;
    viewer                  *pImageViewer   = nullptr;
    DontClick               *pDontClick     = nullptr;
    BellsMonitor            *pBellsMonitor  = nullptr;

//    QWidget                 *centralWidget  = nullptr; //clock date bells_monitor run_string

//FUNCTIONS

    virtual bool event              (QEvent *event);

// для установления общих параметров для всех виджетов
    void    generalSettings         ();
//// просто создание мини виджета с миниатюрой
//    void    createLabelForMiniWidget();

protected:
    void paintEvent(QPaintEvent *);
signals:

private slots:
// удаление объекта из памяти при закрытии окна
    void slotDeleteWidgetInContent();

    void slotWidgetPressed();
    void slotWidgetReleased();
    void slotWidgetClicked();
};

#endif // MINI_WIDGET_H
