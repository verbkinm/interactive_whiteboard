#ifndef MINI_WIDGET_H
#define MINI_WIDGET_H

#include <QWidget>
#include <QLabel>


class Mini_Widget : public QWidget
{
    Q_OBJECT
public:
    Mini_Widget(QString borderColor, QPixmap* miniIcon, QWidget* content, QSize size, QWidget *parent = nullptr);
    //~Mini_Widget();

private:
//рамка, которая будет появлятся при нажатии
    QLabel*                 border;
//центральная миниатюра
    QPixmap*                miniIcon;
//указатель на содержимое
    QWidget*                content;

//FUNCTIONS


signals:

public slots:
};

#endif // MINI_WIDGET_H
