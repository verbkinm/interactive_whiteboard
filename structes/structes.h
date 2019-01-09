#ifndef STRUCTES_H
#define STRUCTES_H

#include <QString>
#include <QRect>
#include <QPoint>
#include <QSize>

enum TYPE_WIDGETS{LABEL, CLOCK, DATE, RUN_STRING, SCHEDULE, IMAGE_VIEWER, DONT_CLICK, BELLS_MONITOR};

int getType(QString typeStr);

struct border
{
    QString borderColor         = "#ffffff";  // цвет рамки
    QString borderClickColor    = "#ff0000";  // цвет появляющийся рамки при нажатии
    int borderWidth             = 10;       // ширина рамки
    int borderClickWidth        = 15;       // ширина появляющийся рамки при нажатии
};

struct text
{
    QString textColor           = "#ffffff";  // цвет текста
    int textSize                = 12;       // размер шрифта

    QString titleColor          = "#ffffff";  // цвет текста
    QString titleText           = "title";     // текст заголовка
    int titleTextSize           = 12;       // размер шрифта заголовка
};

struct path
{
    QString xmlPath             = "\0";     // путь к xml файлу
    QString dirPath             = "\0";     // путь к каталогу
    QString iconPath            = "\0";     // путь к файлу картики
    QString txtPath             = "\0";     // путь к текстовому файлу
};

struct background
{
    QString backgroundColor     = "#ffffff";  // цвет фона
    QString backgroundImage     = "\0";     // картинка фона
};

struct miscellanea
{
    QString type                    = "label"; //тип виджета (label, clock, schedule, image_viewer)
    QString datePattern             = "dd-MM-yy"; // шаблон отображения даты для виджета Date
    int     timerSec                = 0;    //таймер, по которому закрывается виджет - остаётся только главное окно
    bool    dynamicMiniWidget       = false;//является ли виджет динамическим - виджет с заголовком и меняющимися изображениями, а так же с кнопками вперед и назад
    int     dynamicMiniWidgetTimer  = 1;    //таймер для автоматической смены изображений у dynamicMiniWidget
    int     speed                   = -1;
};

struct settingsMiniWidget
{
    struct border       border;
    struct text         text;
    struct path         path;
    struct background   background;
    struct miscellanea  miscellanea;
    QRect               rect = QRect(0,0, 100,100);
    QSize               size = QSize(100, 100);
    QString             widgetName = "new Widget";
    int (*pGetType)(QString) = getType;
};

struct settingsGeneral
{
    struct background   background;
    uint                timerSec;
    bool                fullScreen;
    bool                showCursor;
};

#endif // STRUCTES_H
