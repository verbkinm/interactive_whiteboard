#include "cell.h"

#include <QStringList>

#include <QDebug>

Cell::Cell(QStringList nameOfLesson, QStringList teachers, QStringList roomCabinets) : QObject()
{
    setNameOfLesson(nameOfLesson);
    setTeachers(teachers);
    setRoomCabinets(roomCabinets);
}
Cell::Cell() : QObject()
{

}
void Cell::setNameOfLesson(QStringList nameOfLesson)
{
    this->nameOfLesson.append(nameOfLesson);
}
void Cell::setTeachers(QStringList teacher)
{
    this->teachers.append(teacher);
}
void Cell::setRoomCabinets(QStringList roomCabinet)
{
    this->roomCabinets.append(roomCabinet);
}
QStringList Cell::getnameOfLesson()
{
    return nameOfLesson;
}
QStringList Cell::getTeachers()
{
    return teachers;
}
QStringList Cell::getRoomCabinets()
{
    return roomCabinets;
}
