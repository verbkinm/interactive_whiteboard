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

private:

//файл конфигураций для виджетов, добавляемых на доску
    QSettings                       generals_settings, widget_settings;

//здесь будут храниться указатели на все виджеты на доске
//    QList<QObject*>                 current_widgets;

//указатель на мини виджет
    Mini_Widget *pmini  = nullptr;

//FUNCTIONS
    void            addMyWidgets        ();
    void            addMyWidget         (const QRect &rect, \
                                         const struct border &struct_border, \
                                         const struct miscellanea &struct_miscellanea, \
                                         const struct background &struct_background, \
                                         const struct path &struct_path, \
                                         const struct text &struct_text);


    void            paintEvent          (QPaintEvent*);
    virtual bool    event               (QEvent *event);
};

#endif // MAIN_WIDGET_H
