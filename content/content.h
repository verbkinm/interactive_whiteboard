#ifndef CONTENT_H
#define CONTENT_H

#include <QLabel>

#include "../widget_types/clock.h"

class Content : public QLabel
{
    Q_OBJECT
public:
    Content(QString type, QWidget *parent = nullptr);

private:

        bool        emptyPixmap = true;
        QPixmap*    pPixmap;

        Clock*      pClock;

//        void paintEvent         (QPaintEvent *event);
        virtual bool event      (QEvent *event);

signals:

public slots:
};

#endif // CONTENT_H
