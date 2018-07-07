#ifndef MINI_WIDGET_H
#define MINI_WIDGET_H

#include "widget_types/schedule.h"
#include "widget_types/myWidgets/viewer/viewer.h"
#include "structes/structes.h"

#include <QWidget>
#include <QLabel>
#include <QSize>
#include <QPropertyAnimation>


#include "content/content.h"
#include "structes/structes.h"


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

void createLabelWidget();

void createClockWidget();

void createScheduleWidget();

void createImageViewerWidget();
    //~Mini_Widget();

private:

    enum TYPE_WIDGETS{LABEL, CLOCK, SCHEDULE, IMAGE_VIEWER};
//рамка
    QLabel*                 border          = nullptr;
//рамка, которая будет появлятся при нажатии
    QLabel*                 borderClick     = nullptr;

// указатель на виджет часов
    Clock*                  pClock          = nullptr;

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
// передаваемый через конструктор параметр size, используется в функциях сосдания конкретных виджетов
    QSize _size;

//центральная миниатюра
    QLabel*                 centralLabel    = nullptr;
// центральный виджет в таком типе виджета как clock
    QWidget*                centralWidget   = nullptr;
//указатель на содержимое
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
};

#endif // MINI_WIDGET_H
