#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include <QWidget>
#include <QSettings>

#include "mini_widget/mini_widget.h"
#include "structes/structes.h"

class Main_Widget : public QWidget
{
    Q_OBJECT

public:
    Main_Widget();
    ~Main_Widget();


//файл конфигураций для виджетов, добавляемых на доску
    QSettings                       generals_settings, widget_settings;


private:

//здесь будут храниться указатели на все виджеты на доске
//    QList<QObject*>                 current_widgets;

//указатель на мини виджет
    Mini_Widget *pmini  = nullptr;
//указатель на структуру настроек
    settingsMiniWidget*                 struct_settingsMiniWidget   = nullptr;
//FUNCTIONS
    void            addMyWidgets        ();
    void            addMyWidget         (settingsMiniWidget *struct_settingsMiniWidget, \
                                         QString objectName);


    void            paintEvent          (QPaintEvent*);
    virtual bool    event               (QEvent *event);
};

#endif // MAIN_WIDGET_H
