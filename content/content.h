#ifndef CONTENT_H
#define CONTENT_H

#include <QLabel>
#include <QVBoxLayout>


#include "../widget_types/clock.h"

class Content : public QLabel
{
    Q_OBJECT
public:
    Content(QWidget *parent = nullptr);

    void addWidget(QWidget* w);
private:
    QVBoxLayout* pLayout;

    bool virtual event(QEvent *event);
signals:

public slots:
};

#endif // CONTENT_H
