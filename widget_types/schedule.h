#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QTableWidget>
#include <QDomComment>

class Schedule : public QTableWidget
{
public:
    Schedule(QString xmlPath, QString textColor, unsigned int textSize);

private:
// кол-во тегов lessonTime и class в xml файле
    int     countLesson             = 0;
    int     countClass              = 0;

//заголовок таблицы
    QStringList tableHeader;
//полныть путь файла xml
    QString xmlPath;
    QString textColor;
    unsigned int textSize;

// для подсчета уроков и классов = кол-во столбцов и строк будущей таблицы
    void    countingLessonsAndClasses   (const QDomNode& node);
// парсинг xml файла
    void    parseXml                    (const QDomNode& node);

//настройки таблицы по умолчанию
    void    setDefaultSettings          ();


// ошибка чтения файла xml - кол-во тегов <lesson> превышает кол-во тего <lessonTime>.
// попытка записи данных в не существующие ячейки таблицы
    void    xmlError();
// установка основных свойство таблицы для
};

#endif // SCHEDULE_H
