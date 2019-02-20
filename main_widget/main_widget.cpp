#include "main_widget.h"

#include <QFile>
#include <QPainter>

#include <QDebug>
#include <QEvent>
#include <QTextCodec>

Main_Widget::Main_Widget()
    : QWidget(),   generals_settings(QSettings::IniFormat, QSettings::UserScope, "INTERACTIVE_WHITEBOARD","interactive_whiteboard_generals"), \
                   widget_settings(QSettings::IniFormat, QSettings::UserScope, "INTERACTIVE_WHITEBOARD","interactive_whiteboard_widgets")

{
    generals_settings.setIniCodec("utf8");
    widget_settings.setIniCodec("utf8");

    addMyWidgets();
}
void Main_Widget::addMyWidgets()
{
    QStringList groups = widget_settings.childGroups();

    foreach (QString objectName, groups) {
        widget_settings.beginGroup(objectName);

// структура настроек для мини виджета
    struct_settingsMiniWidget = new settingsMiniWidget;

    struct_settingsMiniWidget->rect.setX(widget_settings.value("x", 10).toInt());
    struct_settingsMiniWidget->rect.setY(widget_settings.value("y", 10).toInt());
    struct_settingsMiniWidget->rect.setWidth(widget_settings.value("width", 100).toInt());
    struct_settingsMiniWidget->rect.setHeight(widget_settings.value("height", 100).toInt());

    struct_settingsMiniWidget->border.borderWidth           = widget_settings.value("borderWidth", 10).toInt();
    struct_settingsMiniWidget->border.borderColor           = widget_settings.value("borderRGBA", "#ffffffff").toString();
    struct_settingsMiniWidget->border.borderClickWidth      = widget_settings.value("borderClickWidth", 10).toInt();
    struct_settingsMiniWidget->border.borderClickColor      = widget_settings.value("borderClickRGBA", "#ff000000").toString();

    struct_settingsMiniWidget->miscellanea.type             = widget_settings.value("type", "label").toString();
    struct_settingsMiniWidget->miscellanea.timerSec         = generals_settings.value("Generals/timerSec", 30).toInt();
    struct_settingsMiniWidget->miscellanea.dynamicMiniWidget= widget_settings.value("dynamicMiniWidget", false).toBool();
    struct_settingsMiniWidget->miscellanea.dynamicMiniWidgetTimer= widget_settings.value("dynamicMiniWidgetTimer", 1).toInt();
    struct_settingsMiniWidget->miscellanea.datePattern      = widget_settings.value("datePattern", "dd-MM-yy").toString();
    struct_settingsMiniWidget->miscellanea.speed            = widget_settings.value("speed", "-1").toInt();

    struct_settingsMiniWidget->background.backgroundColor   = widget_settings.value("backgroundColor", "#ff000000").toString();
    struct_settingsMiniWidget->background.backgroundImage   = generals_settings.value("Generals/backgoundImage", ":img/school2").toString();

    struct_settingsMiniWidget->text.textColor               = widget_settings.value("textColor", "#ff000000").toString();
    struct_settingsMiniWidget->text.textSize                = widget_settings.value("textSize", 12).toInt();
    struct_settingsMiniWidget->text.titleText               = widget_settings.value("title", "\0").toString();
    struct_settingsMiniWidget->text.titleTextSize           = widget_settings.value("titleSize", 12).toInt();
    struct_settingsMiniWidget->text.titleColor              = widget_settings.value("titleColor", "#ff000000").toString();

    struct_settingsMiniWidget->path.dirPath                 = widget_settings.value("dirPath", "\0").toString();
    struct_settingsMiniWidget->path.xmlPath                 = widget_settings.value("xmlPath", "\0").toString();
    struct_settingsMiniWidget->path.iconPath                = widget_settings.value("iconPath", ":img/school2").toString();
    struct_settingsMiniWidget->path.txtPath                 = widget_settings.value("txtPath", "").toString();

    struct_settingsMiniWidget->widgetName                   = objectName;

    widget_settings.endGroup();

    struct_settingsMiniWidget->size = QSize(struct_settingsMiniWidget->rect.width(), struct_settingsMiniWidget->rect.height());
    addMyWidget(struct_settingsMiniWidget, \
                objectName);

    delete struct_settingsMiniWidget;
    }
}
void Main_Widget::addMyWidget(settingsMiniWidget *struct_settingsMiniWidget, \
                              QString objectName)
{
    pmini = new  Mini_Widget(struct_settingsMiniWidget, this);
    pmini->setObjectName(objectName);

    pmini->move(struct_settingsMiniWidget->rect.x(), struct_settingsMiniWidget->rect.y());
    pmini->show();

//    if(pmini != nullptr){
//        pmini->move(struct_settingsMiniWidget.x(),struct_settingsMiniWidget.y());
//        pmini->show();
//    }

}
bool Main_Widget::event(QEvent *event)
{
//    qDebug() << event->type();
    return QWidget::event(event);
}
void Main_Widget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    QPixmap pixMap(generals_settings.value("Generals/backgoundImage", ":img/background").toString());
    QPixmap newPix = pixMap.scaled(this->size(), Qt::IgnoreAspectRatio);
    painter.setBrush(QBrush(Qt::black, newPix));
    painter.drawRect(this->rect());
}
Main_Widget::~Main_Widget()
{
    delete pmini;
}
