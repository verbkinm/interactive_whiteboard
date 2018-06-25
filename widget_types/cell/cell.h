#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QColor>

class Cell : public QObject
{
    Q_OBJECT
private:
    QStringList     nameOfLesson;
    QStringList     teachers;
    QStringList     roomCabinets;

public:
    Cell(QStringList nameOfLesson, QStringList teachers, QStringList roomCabinets);
    Cell();

    void            setNameOfLesson (QStringList nameOfLesson);
    void            setTeachers     (QStringList teacher);
    void            setRoomCabinets (QStringList roomCabinet);

    QStringList     getnameOfLesson ();
    QStringList     getTeachers     ();
    QStringList     getRoomCabinets ();

signals:

public slots:
};

#endif // CELL_H
