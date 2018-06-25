#ifndef MINI_WIDGET_H
#define MINI_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSize>

#include "content/content.h"


class Mini_Widget : public QWidget
{
    Q_OBJECT
public:
    Mini_Widget(QString borderColor, int borderWidth, QString borderClickColor, int borderClickWidth, \
                QPixmap* miniIcon, QSize size, QString type, \
                QString textColor, QString backgroudColor, \
                QString xmlPath,   QWidget *parent);
//type clock
    Mini_Widget(QString borderColor, int borderWidth, QString borderClickColor, int borderClickWidth, \
                QSize size, QString textColor, QString backgroudColor, QWidget *parent);
//type schedule
    Mini_Widget(QString borderColor, int borderWidth, QString borderClickColor, int borderClickWidth, \
                QPixmap* miniIcon, QSize size, \
                QString xmlPath,   QWidget *parent);
    //~Mini_Widget();

private:
    int borderWidth, borderClickWidth;
    QString borderColor, borderClickColor, \
            myType, \
            textColor, backgroudColor, \
//для типа schedule путь к файлу xml
            xmlPath;
//рамка
    QLabel*                 border;
//рамка, которая будет появлятся при нажатии
    QLabel*                 borderClick;
//центральная миниатюра
    QPixmap*                miniIcon;
    QLabel*                 centralLabel;
// центральный виджет в таком типе виджета как clock
    QWidget*                centralWidget;
//указатель на содержимое
    Content*                pContent = 0;

    QSize                   mySize;

//FUNCTIONS

    void    paintEvent              (QPaintEvent*);
    virtual bool event              (QEvent *event);

    void    selectTypeWidget        ();
    void    createLabelForMiniWidget();

signals:

public slots:
};

#endif // MINI_WIDGET_H
