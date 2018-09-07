#ifndef MINI_WIDGET_H
#define MINI_WIDGET_H

#include "widget_types/schedule.h"
#include "widget_types/myWidgets/viewer/viewer.h"
#include "widget_types/clock.h"
#include "widget_types/date.h"
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

    Mini_Widget(const struct border &struct_border, \
                QSize size, \
                const struct path &struct_path, \
                const struct background &struct_background, \
                const struct text &struct_text, \
                const struct miscellanea &struct_miscellanea, \
                QWidget *parent);

    ~Mini_Widget();

// функции для создания виджетов по их типу
void createLabelWidget();
void createClockWidget();
void createDateWidget();
void createScheduleWidget();
void createImageViewerWidget();

private:
    WidgetForMiniWidget *centralWidgetForMiniWidget = nullptr;

    enum TYPE_WIDGETS{LABEL, CLOCK, DATE, SCHEDULE, IMAGE_VIEWER};
//рамка
    QLabel*                 border          = nullptr;
//рамка, которая будет появлятся при нажатии
    QLabel*                 borderClick     = nullptr;


// структура рамки для мини виджета
    struct border struct_border;
// структура свойств текста
    struct text   struct_text;
//структура путей к файлам
    struct path   struct_path;
// структура свойств фона
    struct background struct_background;
// структура разных свойств
    struct miscellanea struct_miscellanea;
// передаваемый через конструктор параметр size, используется в функциях создания конкретных виджетов
    QSize _size;


// указатель для анимаций
    QPropertyAnimation* panimOpen           = nullptr;

//центральная миниатюра
    QLabel*                 centralLabel    = nullptr;
//указатель на содержимое мини виджета, всё что открывается - находится в контейнере класса Content
    Content*                pContent        = nullptr;

//нужно для перевода типа виджета из QString в enum TYPE_WIDGETS
    int                     *type           = nullptr;

//указатели на типы содержиого мини виджета
    Schedule                *pSchedule      = nullptr;
    viewer                  *pImageViewer   = nullptr;

//FUNCTIONS

    virtual bool event              (QEvent *event);

// для установления общих параметров для всех виджетов
    void    generalSettings         ();
// просто создание мини виджета с миниатюрой
    void    createLabelForMiniWidget();

    void    setTypeValue            (QString typeStr);

signals:

private slots:
// удаление объекта из памяти при закрытии окна
    void slotDeleteWidgetInContent();

    void slotWidgetPressed();
    void slotWidgetReleased();
    void slotWidgetClicked();
};

#endif // MINI_WIDGET_H
