#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "myWidgets/mytablewidget.h"
#include "myWidgets/myTabelWidgetEventFilter/mytabelwidgeteventfilter.h"


#include <QWidget>
#include <QGridLayout>
#include <QDomComment>

class Schedule : public QWidget
{
    Q_OBJECT

public:
    Schedule(QString xmlPath, QString textColor, int textSize, QWidget* parent = 0);
    ~Schedule();
private:

    enum ERROR {CANT_OPEN_FILE, SYNTAX_ERROR};
// таблица с номерами уроков и временем
    QTableWidget* tableNumberAndTime    = nullptr;
// таблица с уроками
    QTableWidget* tableLessonData       = nullptr;

    QScrollBar* pScrollHorizontal       = nullptr;
    QScrollBar* pScrollVertical         = nullptr;

    QGridLayout gridLayout;
// кол-во тегов lessonTime и class в xml файле
    int     countLesson             = 0;
    int     countClass              = 0;

//заголовок таблицы
    QStringList tableHeaderNumberAndTime, tableHeaderLessonData;
//полныть путь файла xml
    QString xmlPath;
//цвет текста
    QString textColor;
//размер текста
    int textSize;

    MyTabelWidgetEventFilter* eventFilterTableNumberAndTime = nullptr;
    MyTabelWidgetEventFilter* eventFilterTableLessonData    = nullptr;

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
    void    xmlError(ERROR error);

//если ширина всех столбцов меньше ширины таблицы - растягиваем столбца. Со сроками - аналогично
    void    stretchTable();

    virtual bool event                  (QEvent *event);

signals:
// сигнал передается в класс Content - прерывание таймера бездействия
    void    signalTimerStart();
};

#endif // SCHEDULE_H
