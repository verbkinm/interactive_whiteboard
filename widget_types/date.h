#ifndef DATE_H
#define DATE_H

#include <QLabel>

class Date : public QLabel
{
    Q_OBJECT
public:
    Date(QString textColor, int textSize, QString backgroundColor, QString datePattern, QWidget *parent = nullptr);

private:
    QString textColor, backgroundColor, datePattern;
private slots:
     void slotShowDate();

signals:

public slots:
};

#endif // DATE_H
