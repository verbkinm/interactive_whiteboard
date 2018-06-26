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

//label
    Mini_Widget(QString borderColor, int borderWidth, QString borderClickColor, int borderClickWidth, \
                QPixmap* miniIcon, QSize size, \
                QWidget *parent);
//type clock
    Mini_Widget(QString borderColor, int borderWidth, QString borderClickColor, int borderClickWidth, \
                QSize size, QString textColor, QString backgroudColor, QWidget *parent);
//type schedule
    Mini_Widget(QString borderColor, int borderWidth, QString borderClickColor, int borderClickWidth, \
                QPixmap* miniIcon, QSize size, \
                QString xmlPath,   QString textColor, unsigned int textSize, QWidget *parent);
    //~Mini_Widget();

private:
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


//FUNCTIONS

    void    paintEvent              (QPaintEvent*);
    virtual bool event              (QEvent *event);

// для установления общих параметров для всех виджетов
    void    generalSettings         (QString borderColor, int borderWidth, QString borderClickColor, int borderClickWidth, QSize size);
// просто создание мини виджета с миниатюрой
    void    createLabelForMiniWidget(int borderClickWidth, QPixmap* miniIcon, QSize size);

signals:

public slots:
};

#endif // MINI_WIDGET_H
