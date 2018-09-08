#ifndef RUN_STRING_H
#define RUN_STRING_H

#include <QLabel>
#include <QTimer>

class Run_String : public QLabel
{
    Q_OBJECT
public:
    Run_String(QString textColor, int textSize, QString backgroundColor, QString text, int speed, QWidget *parent = nullptr);
    ~Run_String();
protected:
    virtual void paintEvent(QPaintEvent*event);

private:
    int x, x_end;
    int stringLenght;
    QTimer timer;

    QString str, str_tail;
    QFont *font = nullptr;
    QFontMetrics* fontMetric = nullptr;

private slots:
    void slotRunLine();
};

#endif // RUN_STRING_H
