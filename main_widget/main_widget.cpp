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
    addMyWidgets();
}
void Main_Widget::addMyWidgets()
{
    generals_settings.setIniCodec("utf8");
    widget_settings.setIniCodec("utf8");

    QStringList groups = widget_settings.childGroups();

    foreach (QString str, groups) {
        widget_settings.beginGroup(str);

// структура рамки для мини виджета
        border struct_border;
        text struct_text;
        path struct_path;
        background struct_background;
        miscellanea struct_miscellanea;

//rectangle для мини виджета
        QRect rect;

        rect.setX(widget_settings.value("x", 10).toInt());
        rect.setY(widget_settings.value("y", 10).toInt());
        rect.setWidth(widget_settings.value("width", 50).toInt());
        rect.setHeight(widget_settings.value("height", 50).toInt());

        struct_border.borderWidth           = widget_settings.value("borderWidth", 10).toInt();
        struct_border.borderColor           = widget_settings.value("borderRGBA", "balck").toString();
        struct_border.borderClickWidth      = widget_settings.value("borderClickWidth", 10).toInt();
        struct_border.borderClickColor      = widget_settings.value("borderClickRGBA", "black").toString();

        struct_miscellanea.type             = widget_settings.value("type", "label").toString();
        struct_miscellanea.timerSec         = generals_settings.value("Generals/timerSec", 30).toInt();
        struct_miscellanea.dynamicMiniWidget= widget_settings.value("dynamicMiniWidget", false).toBool();
        struct_miscellanea.dynamicMiniWidgetTimer= widget_settings.value("dynamicMiniWidgetTimer", 5).toInt();
        struct_miscellanea.datePattern      = widget_settings.value("datePattern", "dd-mm-yy").toString();

        struct_background.backgroundColor   = widget_settings.value("backgroundColor", "black").toString();
        struct_background.backgroundImage   = generals_settings.value("Generals/backgoundImage", ":img/school2").toString();

        struct_text.textColor               = widget_settings.value("textColor", "black").toString();
        struct_text.textSize                = widget_settings.value("textSize", 12).toInt();
        struct_text.titleText               = widget_settings.value("title", "\0").toString();

        struct_path.dirPath                 = widget_settings.value("dirPath", "\0").toString();
        struct_path.xmlPath                 = widget_settings.value("xmlPath", "\0").toString();
        struct_path.iconPath                = widget_settings.value("iconPath", ":img/logo").toString();


        widget_settings.endGroup();

//        qDebug() << str << struct_miscellanea.dynamicMiniWidget;

        addMyWidget(rect, \
                    struct_border, \
                    struct_miscellanea, \
                    struct_background,\
                    struct_path, \
                    struct_text);
    }
}
void Main_Widget::addMyWidget(const QRect &rect, \
                              const border &struct_border, \
                              const struct miscellanea &struct_miscellanea, \
                              const struct background &struct_background, \
                              const struct path &struct_path, \
                              const struct text &struct_text)
{
    pmini = new  Mini_Widget(   struct_border, \
                                QSize(rect.width(),rect.height()), \
                                struct_path, \
                                struct_background, \
                                struct_text, \
                                struct_miscellanea, this);

    if(pmini != nullptr){
        pmini->move(rect.x(),rect.y());
        pmini->show();
    }

}
bool Main_Widget::event(QEvent *event)
{
//    qDebug() << event->type();
    return QWidget::event(event);
}
void Main_Widget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    QPixmap pixMap(generals_settings.value("Generals/backgoundImage", ":img/school2").toString());
    QPixmap newPix = pixMap.scaled(this->size(), Qt::IgnoreAspectRatio);
    painter.setBrush(QBrush(Qt::black, newPix));
    painter.drawRect(this->rect());
}
Main_Widget::~Main_Widget()
{
    delete pmini;
}
