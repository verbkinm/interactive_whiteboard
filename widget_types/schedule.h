#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QTableWidget>
#include <QDomDocument>


class Schedule : public QTableWidget
{
public:

    Schedule();

    QDomNode root;

    int numberOfLesson;
    int numberOfCollum;
    int numberOfRow;
    int numberOfClass;

    void            paintEvent          (QPaintEvent*);
};

#endif // SCHEDULE_H
