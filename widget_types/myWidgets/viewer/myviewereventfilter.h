#ifndef MYVIEWEREVENTFILTER_H
#define MYVIEWEREVENTFILTER_H

#include <QObject>

class MyViewerEventFilter : public QObject
{
    Q_OBJECT
public:
    MyViewerEventFilter(QObject *parent = nullptr);

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

#endif // MYVIEWEREVENTFILTER_H
