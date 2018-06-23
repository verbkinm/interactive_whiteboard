#include "main_widget.h"

#include <QFile>
#include <QPainter>

#include <QDebug>
#include <QEvent>
#include <QDesktopWidget>

Main_Widget::Main_Widget()
    : QWidget(), widget_settings("LYCEUM","interactive_whiteboard")
{
    addMyWidgets();
}
void Main_Widget::addMyWidgets()
{
    QStringList groups = widget_settings.childGroups();

    foreach (QString str, groups) {
        qDebug() << str;
        widget_settings.beginGroup(str);
        QStringList keys = widget_settings.childKeys();

        int x,y,width,height,borderWidth,borderClickWidth;
        QString borderRGBA, borderClickRGBA, iconPath, type, textColor, backgroundColor;
        x                   = widget_settings.value("x").toInt();
        y                   = widget_settings.value("y").toInt();
        width               = widget_settings.value("width").toInt();
        height              = widget_settings.value("height").toInt();
        borderWidth         = widget_settings.value("borderWidth").toInt();
        borderRGBA          = widget_settings.value("borderRGBA").toString();
        borderClickWidth    = widget_settings.value("borderClickWidth").toInt();
        borderClickRGBA     = widget_settings.value("borderClickRGBA").toString();
        iconPath            = widget_settings.value("iconPath", ":img/logo").toString();
        type                = widget_settings.value("type").toString();
        textColor           = widget_settings.value("textColor").toString();
        backgroundColor     = widget_settings.value("backgroundColor").toString();
        widget_settings.endGroup();

        addMyWidget(x, y, width, height, borderWidth, borderRGBA, borderClickWidth, \
                    borderClickRGBA, iconPath, type, \
                    textColor, backgroundColor);
    }
}
void Main_Widget::addMyWidget(int x, int y, int width, int height, int borderWidth, \
                              QString borderRGBA, int borderClickWidth, QString borderClickRGBA, QString iconPath, QString type, \
                              QString textColor, QString backgroundColor)
{
    QPixmap* pixmap = new QPixmap(iconPath);
    Mini_Widget* pmini = new Mini_Widget(borderRGBA, borderWidth, borderClickRGBA, borderClickWidth, \
                                         pixmap, QSize(width,height), type, \
                                         textColor, backgroundColor, this);
    pmini->move(x,y);
    pmini->show();

}
bool Main_Widget::event(QEvent *event)
{
//    qDebug() << event->type();
    return QWidget::event(event);
}
void Main_Widget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    QPixmap pix(":img/school2");
    QPixmap newPix = pix.scaled(this->size(), Qt::IgnoreAspectRatio);
    painter.setBrush(QBrush(Qt::black, newPix));
    painter.drawRect(this->rect());
}
Main_Widget::~Main_Widget()
{

}
