#include "main_widget.h"

#include <QFile>
#include <QPainter>

#include <QDebug>
#include <QEvent>
#include <QTextCodec>

Main_Widget::Main_Widget()
    : QWidget(),   generals_settings("LYCEUM","interactive_whiteboard_generals"), \
                   widget_settings("LYCEUM","interactive_whiteboard_widgets")

{
    readGeneralsSettings();

    addMyWidgets();
}
void Main_Widget::addMyWidgets()
{

    QStringList groups = widget_settings.childGroups();

    foreach (QString str, groups) {
//        qDebug() << str;
        widget_settings.beginGroup(str);
//        QStringList keys = widget_settings.childKeys();

        int x,y,width,height,borderWidth,borderClickWidth, textSize;
        QString borderRGBA, borderClickRGBA, \
                iconPath, \
                type, \
                textColor, backgroundColor, \
                xmlPath, \
                dirPath, title;

        x                   = widget_settings.value("x", 10).toInt();
        y                   = widget_settings.value("y", 10).toInt();
        width               = widget_settings.value("width", 50).toInt();
        height              = widget_settings.value("height", 50).toInt();
        borderWidth         = widget_settings.value("borderWidth", 10).toInt();
        borderRGBA          = widget_settings.value("borderRGBA", "#000000").toString();
        borderClickWidth    = widget_settings.value("borderClickWidth", 10).toInt();
        borderClickRGBA     = widget_settings.value("borderClickRGBA", "#000000").toString();
        iconPath            = widget_settings.value("iconPath", ":img/logo").toString();
        type                = widget_settings.value("type", "label").toString();
        textColor           = widget_settings.value("textColor", "black").toString();
        backgroundColor     = widget_settings.value("backgroundColor", "#000000").toString();
        xmlPath             = widget_settings.value("xmlPath", "\0").toString();
        textSize            = widget_settings.value("textSize", 12).toInt();
        dirPath             = widget_settings.value("dirPath", "\0").toString();
        title               = widget_settings.value("title", "\0").toString();

        widget_settings.endGroup();

        addMyWidget(x, y, width, height, borderWidth, borderRGBA, borderClickWidth, \
                    borderClickRGBA, iconPath, type, \
                    textColor, backgroundColor,\
                    xmlPath, textSize, \
                    dirPath, title);
    }
}
void Main_Widget::addMyWidget(int x, int y, int width, int height, int borderWidth, \
                              QString borderRGBA, int borderClickWidth, QString borderClickRGBA, QString iconPath, QString type, \
                              QString textColor, QString backgroundColor, QString xmlPath, unsigned int textSize, \
                              QString dirPath, QString title)
{
    Mini_Widget *pmini  = nullptr;
    QPixmap     *pixmap = nullptr;


    pixmap = new QPixmap(iconPath);
    pmini = new  Mini_Widget(borderRGBA, borderWidth, borderClickRGBA, borderClickWidth, \
                                pixmap, QSize(width,height ), \
                                xmlPath,   dirPath, \
                                textColor, backgroundColor, \
                                title, textSize, type, this);

    if(pmini != nullptr){
        pmini->move(x,y);
        pmini->show();
    }

}
void Main_Widget::readGeneralsSettings()
{
    generals_settings.setIniCodec("utf8");
    widget_settings.setIniCodec("utf8");

    backgoundImage = new QPixmap(generals_settings.value("Generals/backgoundImage", ":img/school2").toString());
}
bool Main_Widget::event(QEvent *event)
{
//    qDebug() << event->type();
    return QWidget::event(event);
}
void Main_Widget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    QPixmap newPix = backgoundImage->scaled(this->size(), Qt::IgnoreAspectRatio);
    painter.setBrush(QBrush(Qt::black, newPix));
    painter.drawRect(this->rect());
}
Main_Widget::~Main_Widget()
{

}
