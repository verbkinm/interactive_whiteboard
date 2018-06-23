#ifndef CLOCK_H
#define CLOCK_H

#include <QLCDNumber>

class Clock : public QLCDNumber
{
    Q_OBJECT
public:
    Clock(QString textColor, QString backgroundColor, QWidget *parent = nullptr);

private:
    QString textColor, backgroundColor;
private slots:
     void slotShowTime();

signals:

public slots:
};

#endif // CLOCK_H
