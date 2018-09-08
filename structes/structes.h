#ifndef STRUCTES_H
#define STRUCTES_H

#include <QString>

struct border
{
    QString borderColor         = "black";  // цвет рамки
    QString borderClickColor    = "black";  // цвет появляющийся рамки при нажатии
    int borderWidth             = 10;       // ширина рамки
    int borderClickWidth        = 10;       // ширина появляющийся рамки при нажатии
};

struct text
{
    QString textColor           = "black";  // цвет текста
    QString titleText           = "\0";     // текст заголовка
    int textSize                = 12;       // размер шрифта
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
    QString backgroundColor     = "black";  // цвет фона
    QString backgroundImage     = "\0";     // картинка фона
};

struct miscellanea
{
    QString type                    = "\0"; //тип виджета (label, clock, schedule, image_viewer)
    QString datePattern             = "dd-mm-yy"; // шаблон отображения даты для виджета Date
    int     timerSec                = 0;    //таймер, по которому закрывается виджет - остаётся только главное окно
    bool    dynamicMiniWidget       = false;//является ли виджет динамическим - виджет с заголовком и меняющимися изображениями, а так же с кнопками вперед и назад
    int     dynamicMiniWidgetTimer  = 5;    //таймер для автоматической смены изображений у dynamicMiniWidget
    int     speed                   = 20;
};

#endif // STRUCTES_H
