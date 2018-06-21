#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include <QWidget>
#include <QSettings>

#include "mini_widget.h"

class Main_Widget : public QWidget
{
    Q_OBJECT

public:
    Main_Widget();
    ~Main_Widget();

//файл конфигураций для виджетов, добавляемых на доску
    QSettings                       widget_settings;

//здесь будут зраниться указатели на все виджеты на доске
    QList<QObject*>                 current_widgets;



//FUNCTIONS
    QObject*        addWidget           ();
    QList<QObject*> addWidgets          ();


};

#endif // MAIN_WIDGET_H
