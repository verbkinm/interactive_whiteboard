#ifndef MYTABELWIDGETEVENTFILTER_H
#define MYTABELWIDGETEVENTFILTER_H

#include <QObject>

#include <QEvent>
#include <QMouseEvent>

class MyTabelWidgetEventFilter : public QObject
{
    Q_OBJECT
public:
    MyTabelWidgetEventFilter(QObject *parent = nullptr);

private:
//координаты курсора
    int x, y;
protected:
    virtual bool eventFilter(QObject* object, QEvent* event);

//имитация touchpada'а
    void slide(QObject* object, QEvent* event);

    enum direction {UP, DOWN};

signals:

public slots:
};

#endif // MYTABELWIDGETEVENTFILTER_H
