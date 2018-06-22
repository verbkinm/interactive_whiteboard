#ifndef MINI_WIDGET_H
#define MINI_WIDGET_H

#include <QWidget>
#include <QLabel>
#include "content/content.h"

class Mini_Widget : public QWidget
{
    Q_OBJECT
public:
    Mini_Widget(QString borderColor, int borderWidth, QString borderClickColor, int borderClickWidth, QPixmap* miniIcon, Content* content, QSize size, QWidget *parent = nullptr);
    //~Mini_Widget();

private:
//рамка
    QLabel*                 border;
//рамка, которая будет появлятся при нажатии
    QLabel*                 borderClick;
//центральная миниатюра
    QPixmap*                miniIcon;
    QLabel*                 centralLabel;
//указатель на содержимое
    Content*                content;

//FUNCTIONS

    void paintEvent         (QPaintEvent *event);
    virtual bool event      (QEvent *event);

signals:

public slots:
};

#endif // MINI_WIDGET_H
