#ifndef CONTENT_H
#define CONTENT_H

#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPushButton>
#include <QTimer>
#include <QTimerEvent>

#include "../widget_types/clock.h"

class Content : public QLabel
{
    Q_OBJECT
public:
    Content(QString title, QString backgoundImagePath, int timerSec = 5, QWidget *parent = nullptr);
    ~Content();

// таймер при истечении которого вызывается слот close()
    QTimer timer;
    int timerSec;

    void            addWidget       (QWidget* w);
    void            setTextSize     (const int &textSize);

private:
    QVBoxLayout*    pLayout     = nullptr;
    QHBoxLayout*    pHLayout    = nullptr;

//
    QLabel*         pTitle      = nullptr;
//указатель на виджет содержимогоs
    QWidget*        pWidget     = nullptr;
//кнопка возврата в главное окно
    QPushButton     home;

// путь к файлу фона виджета
    QPixmap         *backgoundImage = nullptr;

protected:

    bool virtual    event           (QEvent *event);
    void            paintEvent      (QPaintEvent *);

signals:
// вызвать сигнал для родителя Mini_Widget, что бы тот очистил память указателя pSchedule
    void signalClose();
public slots:
    void slotRestartTimer();
};

#endif // CONTENT_H
