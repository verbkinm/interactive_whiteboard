#ifndef CONTENT_H
#define CONTENT_H

#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>


#include "../widget_types/clock.h"

class Content : public QLabel
{
    Q_OBJECT
public:
    Content(QWidget *parent = nullptr);
    ~Content();

    void            addWidget       (QWidget* w);
    void            setTextSize     (const unsigned int &textSize);
private:
    QVBoxLayout*    pLayout= nullptr;

//
    QLabel*         pTitle = nullptr;
//указатель на виджет содержимогоs
    QWidget*        pWidget = nullptr;

    bool virtual    event           (QEvent *event);
    void virtual    childEvent      (QChildEvent* event);
    virtual void    mousePressEvent (QMouseEvent* event);
    void            paintEvent      (QPaintEvent *);
signals:
// вызвать сигнал для родителя Mini_Widget, что бы тот очистил память указателя pSchedule
    void signalClose();
public slots:
};

#endif // CONTENT_H
