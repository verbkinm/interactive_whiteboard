#ifndef CONTENT_H
#define CONTENT_H

#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPushButton>
#include <QTimer>
#include <QTimerEvent>
#include <QPropertyAnimation>

#include "../structes/structes.h"


class Content : public QLabel
{
    Q_OBJECT
public:
    Content(const struct text &struct_text, QString backgoundImagePath, int timerSec, QWidget *parent = nullptr);
    ~Content();

// таймер при истечении которого вызывается слот close()
    QTimer timer;
    int timerSec;

    void            addWidget       (QWidget* w);
    void            setTextSize     (const int &textSize);

    void            setTitle(const QString &title);
private:
    QVBoxLayout*    pLayout     = nullptr;
    QHBoxLayout*    pHLayout    = nullptr;

    struct text struct_text;
//
    QLabel*         pTitle      = nullptr;

//указатель на виджет содержимогоs
    QWidget*        pWidget     = nullptr;
//кнопка возврата в главное окно
    QPushButton     home;

// путь к файлу фона виджета
    QPixmap         *backgoundImage = nullptr;

    QPropertyAnimation* panimClose  = nullptr;

protected:

    bool virtual    event           (QEvent *event);
    void            paintEvent      (QPaintEvent *);


signals:
// вызвать сигнал для родителя Mini_Widget, что бы тот очистил память указателя pSchedule
    void signalClose                ();
public slots:
    void slotRestartTimer           ();
    void slotAnimCloseWindow        ();

};

#endif // CONTENT_H
