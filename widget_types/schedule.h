#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "myWidgets/mytablewidget.h"

#include <QTableWidget>
#include <QGridLayout>
#include <QDomComment>

class Schedule : public QWidget
{
public:
    Schedule(QString xmlPath, QString textColor, unsigned int textSize);

private:

    QTableWidget* tableNumberAndTime    = nullptr;
    QTableWidget* tableLessonData       = nullptr;


    QGridLayout gridLayout;
// кол-во тегов lessonTime и class в xml файле
    int     countLesson             = 0;
    int     countClass              = 0;

//заголовок таблицы
    QStringList tableHeaderNumberAndTime, tableHeaderLessonData;
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
// заливка ячеек цветом
    void    filling                     ();

// делаем высоту строк в двух таблицах одинаковыми
    void    heightSynchronization       ();
// ошибка чтения файла xml - кол-во тегов <lesson> превышает кол-во тего <lessonTime>.
// попытка записи данных в не существующие ячейки таблицы
    void    xmlError();

    virtual bool event                  (QEvent *event);
    void        paintEvent              (QPaintEvent*);
};

#endif // SCHEDULE_H
