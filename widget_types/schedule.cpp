#include "schedule.h"
#include "cell/cell.h"

#include <QFile>
#include <QHeaderView>

#include <QLabel>
#include <QPushButton>

#include <QPainter>
#include <QPalette>

#include <QMessageBox>
#include <QDebug>

#include <QScrollBar>


#define COLOMN_FOR_LESSON_NUMBER    0
#define COLOMN_FOR_TIME             1

#define SEPARATOR                   " - "

// установка данных об уроке(название, кабинет, учитель) в ячейку балицы
#define INSERT_DATA                 Cell* cell = static_cast<Cell*>(tableLessonData->cellWidget(numberLesson, numberOfCurrentColumn)); \
                                    cell->setText(subSubDomElement.text());

Schedule::Schedule(QString xmlPath, QString textColor, int textSize, QWidget *parent) : QWidget(parent)
{ 

    setDefaultSettings();

    this->xmlPath   = xmlPath;
    this->textColor = textColor;
    this->textSize  = textSize;

    QDomDocument domDoc;
    QFile        file(xmlPath);

    if(file.open(QIODevice::ReadOnly)) {
        if(domDoc.setContent(&file)) {
            QDomElement domElement= domDoc.documentElement();
            countingLessonsAndClasses(domElement);

//установка кол-ва столбцов и строк
            tableNumberAndTime->setColumnCount(2);
            tableNumberAndTime->setRowCount(countLesson);

            tableLessonData->setColumnCount(countClass);
            tableLessonData->setRowCount(countLesson);

            parseXml(domElement);
        }
        file.close();
    }
    else
        xmlError(CANT_OPEN_FILE);

//установка хэдэров
    tableNumberAndTime->setHorizontalHeaderLabels(tableHeaderNumberAndTime);
    tableLessonData->setHorizontalHeaderLabels(tableHeaderLessonData);


//растягивание ячеек по содержимому
    tableNumberAndTime->resizeRowsToContents();
//    tableNumberAndTime->resizeColumnsToContents();
    tableLessonData->resizeRowsToContents();
    tableLessonData->resizeColumnsToContents();


//заливка ячеек таблицы
    filling();

//установка компоновщика
    this->setLayout(&gridLayout);

// ширина таблицы с номерами и временем уроков
    tableNumberAndTime->setFixedWidth(tableNumberAndTime->columnWidth(0) + tableNumberAndTime->columnWidth(1));

// синхронизируем высоту строк в твух таблицах
    heightSynchronization();

//снимаем фокус с таблиц
    this->setFocus();

//увеличить размер шрифта на n процентов
    tableNumberAndTime->horizontalHeader()->setStyleSheet("font-size: " + QString::number(textSize + float(textSize) / 100 * 50 ) + "px;");
    tableLessonData->horizontalHeader()->setStyleSheet("font-size: " + QString::number(textSize + float(textSize) / 100 * 50 ) + "px;");

//прозрачный фон таблиц по умолчанию
    tableNumberAndTime->viewport()->setStyleSheet("background: rgba(0,0,0,0%);");
    tableLessonData->viewport()->setStyleSheet("background: rgba(0,0,0,0%);");

//ширина скроллбаров
    pScrollHorizontal->setFixedHeight(tableLessonData->horizontalHeader()->height());
    pScrollVertical->setFixedWidth(tableLessonData->horizontalHeader()->height());

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
    int numberOfCurrentColumn  = 0;

    while(!domNode.isNull()) {
        if(domNode.isElement()) {
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull()) {
// проверка тега lessonTime
                if(domElement.tagName() == "lessonTime"){
// создание пустого виджета в ячейках номера и времени урока
                    Cell* cell_for_number = new Cell(textSize, textColor);
                        tableNumberAndTime->setCellWidget(numberOfCurrentRowForLessonNumberAndTime, COLOMN_FOR_LESSON_NUMBER, cell_for_number);
                    Cell* cell_for_time = new Cell(textSize, textColor);
                        tableNumberAndTime->setCellWidget(numberOfCurrentRowForLessonNumberAndTime, COLOMN_FOR_TIME, cell_for_time);

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
                                    Cell* cell = static_cast<Cell*>(tableNumberAndTime->cellWidget(numberOfCurrentRowForLessonNumberAndTime, COLOMN_FOR_TIME));
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
                    tableHeaderLessonData.append(domElement.attribute("name"));
//внутриности тега lesson
                    QDomNode subNode = domNode.firstChild();// должен быть тег lesson
                    while(!subNode.isNull()){
                        if(subNode.isElement()) {
                            QDomElement subDomElement = subNode.toElement();
                            if(!subDomElement.isNull()) {
                                if(subDomElement.tagName() == "lesson"){
                                    Cell* cell = new Cell(textSize, textColor);
                                    tableLessonData->setCellWidget(numberLesson, numberOfCurrentColumn, cell);
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
                                                    Cell* cell = static_cast<Cell*>(tableLessonData->cellWidget(numberLesson, numberOfCurrentColumn));
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
                            xmlError(SYNTAX_ERROR);
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

    tableNumberAndTime  = new QTableWidget(this);
    tableNumberAndTime->setObjectName("tableNumberAndTime");
    tableLessonData     = new QTableWidget(this);
    tableLessonData->setObjectName("tableLessonData");

    connect(tableLessonData->verticalScrollBar(), SIGNAL(valueChanged(int)), tableNumberAndTime->verticalScrollBar(), SLOT(setValue(int)));

    gridLayout.addWidget(tableNumberAndTime, 0, 0);
    gridLayout.addWidget(tableLessonData, 0, 1);

//отлавливаем все события таблиц
    eventFilterTableNumberAndTime = new MyTabelWidgetEventFilter(tableNumberAndTime->viewport());
    tableNumberAndTime->viewport()->installEventFilter(eventFilterTableNumberAndTime);
//    tableNumberAndTime->horizontalHeader()->viewport()->installEventFilter(new MyTabelWidgetEventFilter(tableNumberAndTime->horizontalHeader()->viewport()));
    eventFilterTableLessonData = new MyTabelWidgetEventFilter(tableLessonData->viewport());
    tableLessonData->viewport()->installEventFilter(eventFilterTableLessonData);
//    tableLessonData->horizontalHeader()->viewport()->installEventFilter(new MyTabelWidgetEventFilter(tableLessonData->horizontalHeader()->viewport()));

    tableHeaderNumberAndTime << "№" << "Врeмя";

    tableNumberAndTime->verticalHeader()->hide();
    tableLessonData->verticalHeader()->hide();

    tableNumberAndTime->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableNumberAndTime->setSelectionMode(QAbstractItemView::NoSelection);
    tableNumberAndTime->setDragDropMode(QAbstractItemView::NoDragDrop);

    tableLessonData->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableLessonData->setSelectionMode(QAbstractItemView::NoSelection);
    tableLessonData->setDragDropMode(QAbstractItemView::NoDragDrop);

    tableNumberAndTime->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableNumberAndTime->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    tableLessonData->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableLessonData->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

// для плавной прокрутки таблицы
    tableLessonData->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    tableLessonData->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    tableNumberAndTime->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

// создаём ссылки на скролл бары таблицы, дабы отделить их своей таблицы
    pScrollHorizontal = tableLessonData->horizontalScrollBar();
    pScrollVertical = tableLessonData->verticalScrollBar();

    gridLayout.addWidget(pScrollHorizontal, 1, 0, 1, 2);
    gridLayout.addWidget(pScrollVertical, 0, 2);
}
void Schedule::filling()
{
    for (int row = 0; row < tableLessonData->rowCount(); row += 2)
        for (int column = 0; column < tableLessonData->columnCount(); ++column) {
            Cell* cell = static_cast<Cell*>(tableLessonData->cellWidget(row,column));
            if(cell != nullptr)
                cell->setBackgroundColor(Cell::LESSON);
        }

    for (int row = 0; row < tableNumberAndTime->rowCount(); ++row) {
        Cell* cell = static_cast<Cell*>(tableNumberAndTime->cellWidget(row,COLOMN_FOR_LESSON_NUMBER));
        if(cell != nullptr)
            cell->setBackgroundColor(Cell::NUMBER);

        cell = static_cast<Cell*>(tableNumberAndTime->cellWidget(row,COLOMN_FOR_TIME));
        if(cell != nullptr)
            cell->setBackgroundColor(Cell::TIME);
    }
}
void Schedule::xmlError(ERROR error)
{
    QMessageBox msgBox;

    switch (error) {
    case CANT_OPEN_FILE:{
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(QString("ОШИБКА!\n"
                               "Не возможно открыть файл " + xmlPath + "!" ));
        qDebug() << "Не возможно открыть файл " + xmlPath + "!";
        }
        break;
    case SYNTAX_ERROR:{
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(QString("ОШИБКА!\n"
                               "Синтаксическая ошибка в файле " + xmlPath + "\n" +
                               "Количество тегов <lesson> привышает количество тегов <lessonTime>"));
        qDebug() << "синтаксическая ошибка в файле " + xmlPath;
        }
        break;
    default:
        break;
    }

    msgBox.exec();
    exit(1);
}
void Schedule::heightSynchronization()
{
// если количество строк в таблицах одинаково, а оно должно быть одинаково, если все нормально с xml файлом
// то устанавливаем высоту строк в двух таблицах одинаковой
    if(tableNumberAndTime->rowCount() == tableLessonData->rowCount()){
        int totalRows = tableNumberAndTime->rowCount();
        for (int row = 0; row < totalRows; ++row) {
            int heightRow_TableNumberAndTime = tableNumberAndTime->rowHeight(row);
            int heightRow_TableLessonData    = tableLessonData->rowHeight(row);

            if(heightRow_TableNumberAndTime < heightRow_TableLessonData)
                tableNumberAndTime->setRowHeight(row, heightRow_TableLessonData);
            else if(heightRow_TableNumberAndTime > heightRow_TableLessonData)
                tableLessonData->setRowHeight(row, heightRow_TableNumberAndTime);
        }
    }
}
void Schedule::stretchTable()
{
// растягиваем ширину столбцов до предела, если ширина всех столбцов меньше ширины тыблицы
    int widthColumns_TableLessonData = 0;
    for (int column = 0; column < tableLessonData->columnCount(); ++column)
        widthColumns_TableLessonData += tableLessonData->columnWidth(column);
    if(widthColumns_TableLessonData < tableLessonData->width()){
        for (int column = 0; column < tableLessonData->columnCount(); ++column)
            tableLessonData->horizontalHeader()->setSectionResizeMode(column, QHeaderView::Stretch);
//        pScrollHorizontal->setDisabled(true);
    }
// растягиваем высоту строк до предела, если высота всех строк меньше высоты тыблицы
    int heightRows_TableLessonData = 0;
    for (int row = 0; row < tableLessonData->rowCount(); ++row)
        heightRows_TableLessonData += tableLessonData->rowHeight(row);
    if(heightRows_TableLessonData < tableLessonData->height()){
        for (int row = 0; row < tableLessonData->rowCount(); ++row){
            tableLessonData->verticalHeader()->setSectionResizeMode(row, QHeaderView::Stretch);
            tableNumberAndTime->verticalHeader()->setSectionResizeMode(row, QHeaderView::Stretch);
        }
//        pScrollVertical->setDisabled(true);
    }
}
bool Schedule::event(QEvent *event)
{
//    qDebug() << event->type();
    emit signalTimerStart();

    if(event->type() == QEvent::Resize)
        stretchTable();

    return QWidget::event(event);
}
Schedule::~Schedule()
{
//    qDebug() << "Schedule destructor";

    for (int row = 0; row < tableNumberAndTime->rowCount(); ++row)
        for (int column = 0; column < tableNumberAndTime->columnCount(); ++column) {
            Cell* cell = static_cast<Cell*>(tableNumberAndTime->cellWidget(row,column));
            delete cell;
        }

    for (int row = 0; row < tableLessonData->rowCount(); ++row)
        for (int column = 0; column < tableLessonData->columnCount(); ++column) {
            Cell* cell = static_cast<Cell*>(tableLessonData->cellWidget(row,column));
            delete cell;
        }

    delete eventFilterTableNumberAndTime;
    delete eventFilterTableLessonData;

    delete tableNumberAndTime;
    delete tableLessonData;

    delete pScrollHorizontal;
    delete pScrollVertical;
}
