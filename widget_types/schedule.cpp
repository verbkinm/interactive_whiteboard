#include "schedule.h"
#include <QFile>
#include <QHeaderView>

#include <QDebug>


// сдвиг для таблицы, первые два столбца зарезервированы под номер и время урока
#define SHIFT 2

#define COLOMN_FOR_LESSON_NUMBER    0
#define COLOMN_FOR_TIME             1

#define SEPARATOR                   " - "

// макрос склеивания времени начала и конца урока и вставки его в ячейку таблицы
#define INSERT_TIME                 if( this->item(numberOfCurrentRowForLessonNumberAndTime, COLOMN_FOR_TIME) == 0 ){ \
                                        QTableWidgetItem* item = new QTableWidgetItem; \
                                        item->setText(subDomElement.text()); \
                                        this->setItem(numberOfCurrentRowForLessonNumberAndTime, COLOMN_FOR_TIME, item); \
                                    }   \
                                    else \
                                    { \
                                        QTableWidgetItem* item = this->item(numberOfCurrentRowForLessonNumberAndTime, COLOMN_FOR_TIME); \
                                        item->setText(item->text() + SEPARATOR + subDomElement.text()); \
                                    }
// установка данных об уроке(название, кабинет, учитель) в ячейку балицы
#define INSERT_DATA                 QTableWidgetItem* item = this->item(numberLesson, numberOfCurrentColumn); \
                                    item->setText(item->text() + subSubDomElement.text() + " ");

Schedule::Schedule(QString xmlPath) : QTableWidget()
{ 
    setDefaultSettings();

    QDomDocument domDoc;
    QFile        file(xmlPath);

    if(file.open(QIODevice::ReadOnly)) {
        if(domDoc.setContent(&file)) {
            QDomElement domElement= domDoc.documentElement();
            countingLessonsAndClasses(domElement);

//установка кол-ва столбцов и строк
            this->setColumnCount(countClass + SHIFT);
            this->setRowCount(countLesson);

            parseXml(domElement);
        }
        file.close();
    }
    else
        qDebug() << "Can't open file " << xmlPath;

    this->setHorizontalHeaderLabels(tableHeader);

    qDebug() << "countLesson = " << countLesson << " countClass = " << countClass;
}
void Schedule::countingLessonsAndClasses(const QDomNode &node)
{
    QDomNode domNode = node.firstChild();

    while(!domNode.isNull()) {
        if(domNode.isElement()) {
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull()) {
// проверка тега lessonTime
                if(domElement.tagName() == "lessonTime")
                    countLesson++;
// проверка тега class
                else if(domElement.tagName() == "class")
                    countClass++;
            }
        }
        domNode = domNode.nextSibling();
    }
}
void Schedule::parseXml(const QDomNode &node)
{
    QDomNode domNode = node.firstChild();

// счетчик для текущей строки, только для столбцов номера урока и времени
    int numberOfCurrentRowForLessonNumberAndTime      = 0;
// счетчик для текущего столбца, используется непосредственно для установки данных урока в ячейку таблицы
    int numberOfCurrentColumn   = 0; numberOfCurrentColumn += SHIFT;

    while(!domNode.isNull()) {
        if(domNode.isElement()) {
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull()) {
// проверка тега lessonTime
                if(domElement.tagName() == "lessonTime"){
// внутриности тега lessonTime
                    QDomNode subNode = domNode.firstChild();
                    while(!subNode.isNull()){
                        if(subNode.isElement()) {
                            QDomElement subDomElement = subNode.toElement();
                            if(!subDomElement.isNull()) {
                                if(subDomElement.tagName() == "number"){
                                    QTableWidgetItem* item = new QTableWidgetItem(subDomElement.text());
                                    this->setItem(numberOfCurrentRowForLessonNumberAndTime, COLOMN_FOR_LESSON_NUMBER, item);
                                }
                                else if(subDomElement.tagName() == "timeBegin"){
                                    INSERT_TIME;
                                }
                                else if(subDomElement.tagName() == "timeEnd"){
                                    INSERT_TIME
                                }
                            }
                        }
                        subNode = subNode.nextSibling();
                    }
                    numberOfCurrentRowForLessonNumberAndTime++;
                }

// проверка тега class
                else if(domElement.tagName() == "class"){
                    int numberLesson = 0;
                    tableHeader.append(domElement.attribute("name"));
//внутриности тега lesson
                    QDomNode subNode = domNode.firstChild();// должен быть тег lesson
                    while(!subNode.isNull()){
                        if(subNode.isElement()) {
                            QDomElement subDomElement = subNode.toElement();
                            if(!subDomElement.isNull()) {
                                if(subDomElement.tagName() == "lesson"){
                                    QTableWidgetItem* item = new QTableWidgetItem;
                                    this->setItem(numberLesson, numberOfCurrentColumn, item);
                                    QDomNode subSubNode = subNode.firstChild(); // должен быть тег name
                                    while ( !subSubNode.isNull() ) {
                                        if(subSubNode.isElement()) {
                                            QDomElement subSubDomElement = subSubNode.toElement();
                                            if(!subSubDomElement.isNull()) {
                                                if(subSubDomElement.tagName() == "name"){
                                                    INSERT_DATA;
                                                }
                                                else if(subSubDomElement.tagName() == "place"){
                                                    INSERT_DATA;
                                                }
                                                else if(subSubDomElement.tagName() == "teacher"){
                                                    INSERT_DATA;
                                                }
                                            }
                                        }
                                        subSubNode = subSubNode.nextSibling();
                                    }
                                }
                            }
                        }
                        numberLesson++;
                        subNode = subNode.nextSibling();
                    }
                    numberOfCurrentColumn++;
                }
            }
        }
        domNode = domNode.nextSibling();
    }
}
void Schedule::setDefaultSettings()
{
    tableHeader << "№" << "Вермя";
//    this->verticalHeader()->hide();
}
