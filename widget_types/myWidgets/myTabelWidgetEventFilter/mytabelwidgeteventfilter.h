#ifndef MYTABELWIDGETEVENTFILTER_H
#define MYTABELWIDGETEVENTFILTER_H

#include <QObject>

#include <QEvent>

class MyTabelWidgetEventFilter : public QObject
{
    Q_OBJECT
public:
    MyTabelWidgetEventFilter(QObject *parent = nullptr);

protected:
    virtual bool eventFilter(QObject* object, QEvent* event);

signals:

public slots:
};

#endif // MYTABELWIDGETEVENTFILTER_H
