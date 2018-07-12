#ifndef FINGERSLIDE_H
#define FINGERSLIDE_H

#include <QObject>

#include <QEvent>
#include <QMouseEvent>

class FingerSlide : public QObject
{
    Q_OBJECT
public:
    FingerSlide(QObject *parent = nullptr);

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

#endif // FINGERSLIDE_H
