#ifndef FINGERSLIDE_H
#define FINGERSLIDE_H

#include <QObject>

#include <QTime>
#include <QTimer>
#include <QEvent>
#include <QMouseEvent>

class FingerSlide : public QObject
{
    Q_OBJECT
public:
    FingerSlide(QObject *parent = nullptr);
    ~FingerSlide();

private:
//координаты курсора
    int x, y;

    int preX, preY;
    int stepX, stepY;

    QTimer Tt, Ts;

    QObject* object = nullptr;

protected:
    virtual bool eventFilter(QObject* object, QEvent* event);

//имитация touchpada'а
    void slide(QObject* object, QEvent* event);
//инерция
    void inertion(QObject* object, QEvent* event);

signals:

public slots:
    void slotInertion();
};

#endif // FINGERSLIDE_H
