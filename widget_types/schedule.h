#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QTableWidget>
#include <QDomComment>

class Schedule : public QTableWidget
{
public:
    Schedule(QString xmlPath);

private:
// кол-во тегов lessonTime и class в xml файле
    int     countLesson             = 0;
    int     countClass              = 0;

    QStringList tableHeader;

// для подсчета уроков и классов = кол-во столбцов и строк будущей таблицы
    void    countingLessonsAndClasses   (const QDomNode& node);
// парсинг xml файла
    void    parseXml                    (const QDomNode& node);

//настройки таблицы по умолчанию
    void    setDefaultSettings          ();
};

#endif // SCHEDULE_H
