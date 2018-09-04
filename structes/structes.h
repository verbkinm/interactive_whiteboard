#ifndef STRUCTES_H
#define STRUCTES_H

#include <QString>

struct border
{
    QString borderColor         = "black";
    QString borderClickColor    = "black";
    int borderWidth             = 10;
    int borderClickWidth        = 10;
};

struct text
{
    QString textColor           = "black";
    QString titleText           = "\0";
    int textSize                = 12;
};

struct path
{
    QString xmlPath             = "\0";
    QString dirPath             = "\0";
    QString iconPath            = "\0";
};

struct background
{
    QString backgroundColor     = "black";
    QString backgroundImage     = "\0";
};

struct miscellanea
{
    QString type                    = "\0";
//таймер, по которому виджет закрывается
    int     timerSec                = 0;
//виджет с заголовком и меняющимися изображениями, а так же с кнопками вперед и назад
    bool    dynamicMiniWidget       = false;
//таймер для автоматической смены изображений у dynamicMiniWidget
    int     dynamicMiniWidgetTimer  = 5;
};

#endif // STRUCTES_H
