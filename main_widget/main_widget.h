#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include <QWidget>
#include <QSettings>

#include "mini_widget/mini_widget.h"

class Main_Widget : public QWidget
{
    Q_OBJECT

public:
    Main_Widget();
    ~Main_Widget();

private:

//файл конфигураций для виджетов, добавляемых на доску
    QSettings                       generals_settings, widget_settings;

//картинка фона главного окна
    QPixmap*                        backgoundImage = nullptr;

//здесь будут зраниться указатели на все виджеты на доске
    QList<QObject*>                 current_widgets;



//FUNCTIONS
    void            addMyWidgets        ();
    void            addMyWidget         (int x, int y, int width, int height, int borderWidth, \
                                         QString  borderRGBA, int borderClickWidth, QString borderClickRGBA, QString iconPath, QString type, \
                                         QString textColor, QString backgroundColor, QString xmlPath, unsigned int textSize);

    void            readGeneralsSettings();

    void            paintEvent          (QPaintEvent*);
    virtual bool    event               (QEvent *event);
};

#endif // MAIN_WIDGET_H
