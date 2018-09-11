#ifndef DONTCLICK_H
#define DONTCLICK_H

#include <QWidget>
#include <QLabel>

class DontClick : public QLabel
{
    Q_OBJECT
public:
    DontClick(int textSize, QString textColor, QString backgroundColor, QWidget *parent = nullptr);

    void setCountPush(int count);

private:

signals:

public slots:
};

#endif // DONTCLICK_H
