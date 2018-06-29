#ifndef MYTABLEWIDGET_H
#define MYTABLEWIDGET_H

#include <QTableWidget>

class MyTableWidget : public QTableWidget
{
public:
    MyTableWidget();

private:
    virtual bool event(QEvent *e);
};

#endif // MYTABLEWIDGET_H
