#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QTableWidget>
#include "myWidgets/fingerslide/fingerslide.h"


#include <QWidget>
#include <QGridLayout>
#include <QDomComment>

class Schedule : public QWidget
{
    Q_OBJECT

public:
    Schedule(QString xmlPath, QString textColor, int textSize, QWidget* parent = 0);
    ~Schedule();

    QString getTitle();

    bool CRITICAL_ERROR = false;

private:

    enum ERROR {CANT_OPEN_FILE, SYNTAX_ERROR};

//таблица-хедер номеров уроков и времени
    QTableWidget* tableHeaderNumberTime     = nullptr;
//таблица-хедер с назваиями классов
    QTableWidget* tableHeaderClassName      = nullptr;
// таблица с номерами уроков и временем
    QTableWidget* tableNumberAndTime        = nullptr;
// таблица с уроками
    QTableWidget* tableLessonData           = nullptr;

    QScrollBar* pScrollHorizontal           = nullptr;
    QScrollBar* pScrollVertical             = nullptr;

// компоновщик
    QGridLayout gridLayout;

// кол-во тегов lessonTime и class в xml файле
    int     countLesson             = 0;
    int     countClass              = 0;

//заголовоки таблиц
    QStringList tableHeaderNumberAndTime, tableHeaderLessonData;
//полныть путь файла xml
    QString xmlPath;
//цвет текста
    QString textColor;
//размер текста
    int textSize;
//титул
    QString title;

// фильтры событий для таблиц
    FingerSlide* eventFilterTableNumberAndTime = nullptr;
    FingerSlide* eventFilterTableLessonData    = nullptr;
    FingerSlide* eventFilterTableClassName     = nullptr;

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
// делаем ширину столбоц в таблицах одинаковыми
    void    widthSynchronization        ();
// ошибка чтения файла xml - кол-во тегов <lesson> превышает кол-во тего <lessonTime>.
    // попытка записи данных в не существующие ячейки таблицы
    void    xmlError(ERROR error);

//если ширина всех столбцов меньше ширины таблицы - растягиваем столбца. Со сроками - аналогично
    void    stretchTable();
// установка псевдо-хедера таблицы
    void    setHeaderForTables();

    virtual bool event                  (QEvent *event);

signals:
// сигнал передается в класс Content - прерывание таймера бездействия
    void    signalTimerStart();
    void    signalErrorClose();
};

#endif // SCHEDULE_H
