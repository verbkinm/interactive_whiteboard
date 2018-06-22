#include "main_widget.h"

#include <QFile>
#include <QDebug>

Main_Widget::Main_Widget()
    : QWidget(), widget_settings("LYCEUM","interactive_whiteboard")
{
    addMyWidgets();



    this->setStyleSheet("background-color:green;");
    qDebug() << "this" << backgroundRole()<< foregroundRole();
}
void Main_Widget::addMyWidgets()
{
    QStringList groups = widget_settings.childGroups();

    foreach (QString str, groups) {
        qDebug() << str;
        widget_settings.beginGroup(str);
        QStringList keys = widget_settings.childKeys();

        int x,y,width,height,borderWidth,borderClickWidth;
        QString borderRGBA, borderClickRGBA, iconPath, type;
        x                   = widget_settings.value("x").toInt();
        y                   = widget_settings.value("y").toInt();
        width               = widget_settings.value("width").toInt();
        height              = widget_settings.value("height").toInt();
        borderWidth         = widget_settings.value("borderWidth").toInt();
        borderRGBA          = widget_settings.value("borderRGBA").toString();
        borderClickWidth    = widget_settings.value("borderClickWidth").toInt();
        borderClickRGBA     = widget_settings.value("borderClickRGBA").toString();
        iconPath            = widget_settings.value("iconPath").toString();
        type                = widget_settings.value("type").toString();
        widget_settings.endGroup();

        addMyWidget(x, y, width, height, borderWidth, borderRGBA, borderClickWidth, borderClickRGBA, iconPath, type);
    }
}
void Main_Widget::addMyWidget(int x, int y, int width, int height, int borderWidth, QString borderRGBA, int borderClickWidth, QString borderClickRGBA, QString iconPath, QString type)
{
    qDebug() << x << y<< width << height << borderWidth << borderRGBA << borderClickWidth << borderClickRGBA << iconPath;
    QPixmap* pixmap = new QPixmap(iconPath);
    Mini_Widget* pmini = new Mini_Widget(borderRGBA, borderWidth, borderClickRGBA, borderClickWidth, pixmap, new Content(type), QSize(width,height), this);
    pmini->move(x,y);
    pmini->show();

}
Main_Widget::~Main_Widget()
{

}
