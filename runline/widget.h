#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    virtual void paintEvent(QPaintEvent*event);
    virtual bool event(QEvent *event);

private:
    int x1, x_end, x2;
    int stringLenght;
    QTimer timer;

    QString str, str_tail;
    QFont *font = nullptr;
    QFontMetrics* fontMetric = nullptr;

private slots:
    void slotRunLine();
};

#endif // WIDGET_H
