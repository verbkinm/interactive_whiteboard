#ifndef CLOCK_H
#define CLOCK_H

#include <QLCDNumber>

class Clock : public QLCDNumber
{
    Q_OBJECT
public:
    Clock(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // CLOCK_H
