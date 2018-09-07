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
// +1 для эмитации хедера первой строкой
            tableNumberAndTime->setRowCount(countLesson);

            tableLessonData->setColumnCount(countClass);
// +1 для эмитации хедера первой строкой
            tableLessonData->setRowCount(countLesson);

// таблицы хедеры
            tableHeaderNumberTime->setRowCount(1);
            tableHeaderNumberTime->setColumnCount(tableNumberAndTime->columnCount());

            tableHeaderClassName->setRowCount(tableHeaderNumberTime->rowCount());
            tableHeaderClassName->setColumnCount(tableLessonData->columnCount());

            parseXml(domElement);
        }
        file.close();
    }
    else
        xmlError(CANT_OPEN_FILE);

//установка хэдэров
    setHeaderForTables();

//растягивание ячеек по содержимому
    tableNumberAndTime->resizeRowsToContents();
    tableNumberAndTime->resizeColumnsToContents();
    tableLessonData->resizeRowsToContents();
    tableLessonData->resizeColumnsToContents();

    tableHeaderNumberTime->resizeRowsToContents();
    tableHeaderNumberTime->resizeColumnsToContents();
    tableHeaderClassName->resizeRowsToContents();
    tableHeaderClassName->resizeColumnsToContents();

//заливка ячеек таблицы
    filling();

//установка компоновщика
    this->setLayout(&gridLayout);

// размер высоты таблиц-хедеров
    tableHeaderNumberTime->setFixedHeight(tableHeaderNumberTime->rowHeight(0) + tableHeaderNumberTime->rowHeight(1) + 2);
    tableHeaderClassName->setFixedHeight(tableHeaderClassName->rowHeight(0) + tableHeaderClassName->rowHeight(1) + 2);

// синхронизируем высоту  и ширины в таблицах
    heightSynchronization();
    widthSynchronization();

//снимаем фокус с таблиц
    this->setFocus();

//прозрачный фон таблиц по умолчанию
    tableNumberAndTime->viewport()->setStyleSheet("background: rgba(0,0,0,0%);");
    tableLessonData->viewport()->setStyleSheet("background: rgba(0,0,0,0%);");

//ширина скроллбаров
    pScrollHorizontal->setFixedHeight(tableHeaderClassName->height());
    pScrollVertical->setFixedWidth(tableHeaderClassName->height());
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
                else if(domElement.tagName() == "title")
                    this->title = domNode.toElement().text();
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
// создание таблиц
    tableNumberAndTime          = new QTableWidget(this);
    tableNumberAndTime->setObjectName("tableNumberAndTime");
    tableLessonData             = new QTableWidget(this);
    tableLessonData->setObjectName("tableLessonData");

    tableHeaderNumberTime    = new QTableWidget(this);
    tableHeaderNumberTime->setObjectName("tableHeaderNumberAndTime");
    tableHeaderClassName        = new QTableWidget(this);
    tableHeaderClassName->setObjectName("tableHeaderClassName");


    connect(tableLessonData->verticalScrollBar(), SIGNAL(valueChanged(int)), \
            tableNumberAndTime->verticalScrollBar(), SLOT(setValue(int)));
    connect(tableNumberAndTime->verticalScrollBar(), SIGNAL(valueChanged(int)), \
            tableLessonData->verticalScrollBar(), SLOT(setValue(int)));

    connect(tableLessonData->horizontalScrollBar(), SIGNAL(valueChanged(int)), \
            tableHeaderClassName->horizontalScrollBar(), SLOT(setValue(int)));
    connect(tableHeaderClassName->horizontalScrollBar(), SIGNAL(valueChanged(int)), \
            tableLessonData->horizontalScrollBar(), SLOT(setValue(int)));

    gridLayout.addWidget(tableHeaderNumberTime, 0, 0);
    gridLayout.addWidget(tableHeaderClassName, 0, 1);

    gridLayout.addWidget(tableNumberAndTime, 1, 0);
    gridLayout.addWidget(tableLessonData, 1, 1);
    gridLayout.setSpacing(0);

//отлавливаем все события таблиц
    eventFilterTableNumberAndTime = new FingerSlide(tableNumberAndTime->viewport());
    tableNumberAndTime->viewport()->installEventFilter(eventFilterTableNumberAndTime);

    eventFilterTableLessonData = new FingerSlide(tableLessonData->viewport());
    tableLessonData->viewport()->installEventFilter(eventFilterTableLessonData);

    eventFilterTableClassName = new FingerSlide(tableHeaderClassName->viewport());
    tableHeaderClassName->viewport()->installEventFilter(eventFilterTableClassName);

    tableHeaderNumberAndTime << "№" << "Врeмя";

// скрытие встроеных хедеров
    tableNumberAndTime->verticalHeader()->hide();
    tableNumberAndTime->horizontalHeader()->hide();
    tableLessonData->verticalHeader()->hide();
    tableLessonData->horizontalHeader()->hide();

    tableHeaderNumberTime->verticalHeader()->hide();
    tableHeaderNumberTime->horizontalHeader()->hide();

    tableHeaderClassName->verticalHeader()->hide();
    tableHeaderClassName->horizontalHeader()->hide();

// отключение редактирования данных ячеек
    tableNumberAndTime->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableNumberAndTime->setSelectionMode(QAbstractItemView::NoSelection);
    tableNumberAndTime->setDragDropMode(QAbstractItemView::NoDragDrop);

    tableLessonData->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableLessonData->setSelectionMode(QAbstractItemView::NoSelection);
    tableLessonData->setDragDropMode(QAbstractItemView::NoDragDrop);

    tableHeaderNumberTime->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableHeaderNumberTime->setSelectionMode(QAbstractItemView::NoSelection);
    tableHeaderNumberTime->setDragDropMode(QAbstractItemView::NoDragDrop);

    tableHeaderClassName->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableHeaderClassName->setSelectionMode(QAbstractItemView::NoSelection);
    tableHeaderClassName->setDragDropMode(QAbstractItemView::NoDragDrop);

// скрытие родных скроллбаров
    tableNumberAndTime->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableNumberAndTime->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    tableLessonData->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableLessonData->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    tableHeaderNumberTime->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableHeaderNumberTime->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    tableHeaderClassName->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableHeaderClassName->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

// для плавной прокрутки таблицы
    tableLessonData->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    tableLessonData->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    tableNumberAndTime->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    tableHeaderClassName->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

// создаём ссылки на скролл бары таблицы, дабы отделить их от своей таблицы
    pScrollHorizontal = tableLessonData->horizontalScrollBar();
    pScrollVertical = tableLessonData->verticalScrollBar();

    gridLayout.addWidget(pScrollHorizontal, 2, 0, 1, 2);
    gridLayout.addWidget(pScrollVertical,   0, 2, 2,1);

}
void Schedule::filling()
{
// tableLessonData
    for (int row = 0; row < tableLessonData->rowCount(); row += 2)
        for (int column = 0; column < tableLessonData->columnCount(); ++column) {
            Cell* cell = static_cast<Cell*>(tableLessonData->cellWidget(row,column));
            if(cell != nullptr)
                cell->setBackgroundColor(Cell::LESSON);
        }
//tableLessonData HEADER
        for (int column = 0; column < tableHeaderLessonData.length(); ++column) {
            Cell* cell = static_cast<Cell*>(tableHeaderClassName->cellWidget(0,column));
            if(cell != nullptr)
                cell->setBackgroundColor(Cell::HEADER);
        }

//tableNumberAndTime
    for (int row = 0; row < tableNumberAndTime->rowCount(); ++row) {
        Cell* cell = static_cast<Cell*>(tableNumberAndTime->cellWidget(row,COLOMN_FOR_LESSON_NUMBER));
        if(cell != nullptr)
            cell->setBackgroundColor(Cell::NUMBER);

        cell = static_cast<Cell*>(tableNumberAndTime->cellWidget(row,COLOMN_FOR_TIME));
        if(cell != nullptr)
            cell->setBackgroundColor(Cell::TIME);
    }
//tableNumberAndTime HEADER
    for (int column = 0; column < tableHeaderNumberAndTime.length(); ++column) {
        Cell* cell = static_cast<Cell*>(tableHeaderNumberTime->cellWidget(0,column));
        if(cell != nullptr)
            cell->setBackgroundColor(Cell::HEADER);
    }

}
void Schedule::xmlError(ERROR error)
{
    CRITICAL_ERROR = true;

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
    emit signalErrorClose();
//    exit(1);
}
void Schedule::heightSynchronization()
{
// если количество строк в таблицах одинаково, а оно должно быть одинаково, если все нормально с xml файлом,
// то устанавливаем высоту строк в таблицах одинаковой
    if(tableNumberAndTime->rowCount() == tableLessonData->rowCount())
        for (int row = 0; row < tableNumberAndTime->rowCount(); ++row) {
            int heightRow_TableNumberAndTime = tableNumberAndTime->rowHeight(row);
            int heightRow_TableLessonData    = tableLessonData->rowHeight(row);

            if(heightRow_TableNumberAndTime < heightRow_TableLessonData)
                tableNumberAndTime->setRowHeight(row, heightRow_TableLessonData);
            else if(heightRow_TableNumberAndTime > heightRow_TableLessonData)
                tableLessonData->setRowHeight(row, heightRow_TableNumberAndTime);
        }

    if(tableHeaderNumberTime->rowCount() == tableHeaderClassName->rowCount())
        for (int row = 0; row < tableHeaderNumberTime->rowCount(); ++row) {
            int heightRow_tableHeaderNumberTime = tableHeaderNumberTime->rowHeight(row);
            int heightRow_tableHeaderClassName  = tableHeaderClassName->rowHeight(row);

            if(heightRow_tableHeaderNumberTime < heightRow_tableHeaderClassName)
                tableHeaderNumberTime->setRowHeight(row, heightRow_tableHeaderClassName);
            else if(heightRow_tableHeaderNumberTime > heightRow_tableHeaderClassName)
                tableHeaderClassName->setRowHeight(row, heightRow_tableHeaderNumberTime);
        }
}
void Schedule::widthSynchronization()
{
// левые таблицы - номер и время урока
    if(tableHeaderNumberTime->columnCount() == tableNumberAndTime->columnCount())
        for (int column = 0; column < tableHeaderNumberTime->columnCount(); ++column) {
            int widthColumn_tableHeaderNumberTime = tableHeaderNumberTime->columnWidth(column);
            int widthColumn_tableNumberAndTime    = tableNumberAndTime->columnWidth(column);

            if(widthColumn_tableHeaderNumberTime < widthColumn_tableNumberAndTime)
                tableHeaderNumberTime->setColumnWidth(column, widthColumn_tableNumberAndTime);
            else if(widthColumn_tableHeaderNumberTime > widthColumn_tableNumberAndTime)
                tableNumberAndTime->setColumnWidth(column, widthColumn_tableHeaderNumberTime);
        }
        tableNumberAndTime->setFixedWidth(tableNumberAndTime->columnWidth(0) + tableNumberAndTime->columnWidth(1) + 2);
        tableHeaderNumberTime->setFixedWidth(tableHeaderNumberTime->columnWidth(0) + tableHeaderNumberTime->columnWidth(1) + 2);

// правые таблицы - названия классов и уроки
    if(tableHeaderClassName->columnCount() == tableLessonData->columnCount())
        for (int column = 0; column < tableHeaderClassName->columnCount(); ++column) {
            int widthColumn_tableHeaderClassName    = tableHeaderClassName->columnWidth(column);
            int widthColumn_tableLessonData         = tableLessonData->columnWidth(column);

            if(widthColumn_tableHeaderClassName < widthColumn_tableLessonData)
                tableHeaderClassName->setColumnWidth(column, widthColumn_tableLessonData);
            else if(widthColumn_tableHeaderClassName > widthColumn_tableLessonData)
                tableLessonData->setColumnWidth(column, widthColumn_tableHeaderClassName);
        }
}
void Schedule::stretchTable()
{
// растягиваем ширину столбцов до предела, если ширина всех столбцов меньше ширины тыблицы
    int widthColumns_TableLessonData = 0;
    for (int column = 0; column < tableLessonData->columnCount(); ++column)
        widthColumns_TableLessonData += tableLessonData->columnWidth(column);
    if(widthColumns_TableLessonData < tableLessonData->width()){
        for (int column = 0; column < tableLessonData->columnCount(); ++column){
            tableLessonData->horizontalHeader()->setSectionResizeMode(column, QHeaderView::Stretch);
            tableHeaderClassName->horizontalHeader()->setSectionResizeMode(column, QHeaderView::Stretch);
        }
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
    }
}
QString Schedule::getTitle()
{
    return title;
}
void Schedule::setHeaderForTables()
{
    Cell* cell;
    int headerTextSize = textSize + float(textSize) / 100 * 50;
// tableHeaderNumberTime
    for (int column = 0; column < tableHeaderNumberAndTime.length(); ++column) {
        cell = new Cell(headerTextSize, textColor, true);
        tableHeaderNumberTime->setCellWidget(0, column, cell);
        cell->setText(tableHeaderNumberAndTime.at(column));
    }
// tableHeaderClassName
    for (int column = 0; column < tableHeaderLessonData.length(); ++column) {
        cell = new Cell(headerTextSize, textColor, true);
        tableHeaderClassName->setCellWidget(0, column, cell);
        cell->setText(tableHeaderLessonData.at(column));
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
            cell->~Cell();
        }

    for (int row = 0; row < tableLessonData->rowCount(); ++row)
        for (int column = 0; column < tableLessonData->columnCount(); ++column) {
            Cell* cell = static_cast<Cell*>(tableLessonData->cellWidget(row,column));
            cell->~Cell();
        }

    delete eventFilterTableNumberAndTime;
    delete eventFilterTableLessonData;

    delete tableNumberAndTime;
    delete tableLessonData;

    delete pScrollHorizontal;
    delete pScrollVertical;
}
