#include "schedule.h"
#include "cell/cell.h"

#include <QFile>
#include <QHeaderView>

#include <QLabel>
#include <QPushButton>

#include <QMessageBox>
#include <QDebug>


// сдвиг для таблицы, первые два столбца зарезервированы под номер и время урока
#define SHIFT 2

#define COLOMN_FOR_LESSON_NUMBER    0
#define COLOMN_FOR_TIME             1

#define SEPARATOR                   " - "

// макрос склеивания времени начала и конца урока и вставки его в ячейку таблицы
#define INSERT_TIME                 if( this->item(numberOfCurrentRowForLessonNumberAndTime, COLOMN_FOR_TIME) == 0 ){ \
                                        QTableWidgetItem* item = new QTableWidgetItem; \
                                        QFont font = item->font(); \
                                        font.setPixelSize(textSize); \
                                        item->setFont(font); \
                                        item->setTextAlignment(Qt::AlignCenter); \
                                        item->setText(subDomElement.text()); \
                                        this->setItem(numberOfCurrentRowForLessonNumberAndTime, COLOMN_FOR_TIME, item); \
                                    }   \
                                    else \
                                    { \
                                        QTableWidgetItem* item = this->item(numberOfCurrentRowForLessonNumberAndTime, COLOMN_FOR_TIME); \
                                        item->setTextAlignment(Qt::AlignCenter); \
                                        item->setText(item->text() + SEPARATOR + subDomElement.text()); \
                                    }
// установка данных об уроке(название, кабинет, учитель) в ячейку балицы
#define INSERT_DATA                 Cell* cell = static_cast<Cell*>(this->cellWidget(numberLesson, numberOfCurrentColumn)); \
                                    cell->setText(subSubDomElement.text());

Schedule::Schedule(QString xmlPath, QString textColor, unsigned int textSize) : QTableWidget()
{ 
    this->xmlPath   = xmlPath;
    this->textColor = textColor;
    this->textSize  = textSize;

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

    this->resizeRowsToContents();
    this->resizeColumnsToContents();
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
    int numberOfCurrentColumn  = SHIFT;

    while(!domNode.isNull()) {
        if(domNode.isElement()) {
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull()) {
// проверка тега lessonTime
                if(domElement.tagName() == "lessonTime"){
// создание пустого виджета в ячейках номера и времени урока
                    Cell* cell_for_number = new Cell(textSize, textColor);
                        this->setCellWidget(numberOfCurrentRowForLessonNumberAndTime, COLOMN_FOR_LESSON_NUMBER, cell_for_number);
                    Cell* cell_for_time = new Cell(textSize, textColor);
                        this->setCellWidget(numberOfCurrentRowForLessonNumberAndTime, COLOMN_FOR_TIME, cell_for_time);

                    QDomNode subNode = domNode.firstChild();
// внутриности тега lessonTime
                    while(!subNode.isNull()){
                        if(subNode.isElement()) {
                            QDomElement subDomElement = subNode.toElement();
                            if(!subDomElement.isNull()) {
                                if(subDomElement.tagName() == "number")
// заполнение ячейки с номером урока
                                    cell_for_number->setText(subDomElement.text());
// заполнение ячейки со временем урока
                                else if(subDomElement.tagName() == "timeBegin")
                                    cell_for_time->setText(subDomElement.text());
// заполнение ячейки со временем урока
                                else if(subDomElement.tagName() == "timeEnd")
                                    cell_for_time->setText(subDomElement.text());
                                else if(subDomElement.tagName() == "separator"){
                                    Cell* cell = static_cast<Cell*>(this->cellWidget(numberOfCurrentRowForLessonNumberAndTime, COLOMN_FOR_TIME));
                                    cell->separate();
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
                                    Cell* cell = new Cell(textSize, textColor);
                                    this->setCellWidget(numberLesson, numberOfCurrentColumn, cell);
                                    QDomNode subSubNode = subNode.firstChild(); // должен быть тег name
                                    while ( !subSubNode.isNull() ) {
                                        if(subSubNode.isElement()) {
                                            QDomElement subSubDomElement = subSubNode.toElement();
                                            if(!subSubDomElement.isNull()) {
                                                if(subSubDomElement.tagName() == "name"){
                                                    INSERT_DATA
                                                }
                                                else if(subSubDomElement.tagName() == "place"){
                                                    INSERT_DATA
                                                }
                                                else if(subSubDomElement.tagName() == "teacher"){
                                                    INSERT_DATA
                                                }
                                                else if(subSubDomElement.tagName() == "separator"){
                                                    Cell* cell = static_cast<Cell*>(this->cellWidget(numberLesson, numberOfCurrentColumn));
                                                    cell->separate();
                                                }
                                            }
                                        }
                                        subSubNode = subSubNode.nextSibling();
                                    }
                                }
                            }
                        }
                        numberLesson++;
                        if(numberLesson > countLesson)
                            xmlError();
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

    this->horizontalHeader()->setStyleSheet("font-size: " + QString::number(textSize) + "px;");

    this->verticalHeader()->hide();
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->setSelectionMode(QAbstractItemView::NoSelection);
    this->setDragDropMode(QAbstractItemView::NoDragDrop);

//    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
void Schedule::xmlError()
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText(QString("ОШИБКА!\n"
                           "Синтаксическая ошибка в файле " + xmlPath + "\n" +
                           "Количество тегов <lesson> привышает количество тегов <lessonTime>"));
    qDebug() << "синтаксическая ошибка в файле " + xmlPath;
    msgBox.exec();
    exit(1);
}
