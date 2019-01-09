#include "bellsmonitor.h"

#include <QDebug>
#include <QDate>
#include <QPixmap>
#include <QFile>
#include <QFont>

#define textColor               "#000000"
#define backgroundColor         "#ffffff"
#define SelectBackgroundColor   QColor(211,211,211,215)
#define SelectTextColor         "#FF0000"
#define headerBackgroundColor   QColor(144, 238, 144,200)
#define backgroundColorAlpha    QColor(255,255,255,230)

#define timerPingDuration       5000
#define timerWaitReconnect      1000

#define dash        "-- : --"

BellsMonitor::BellsMonitor(QWidget *parent) :
    QWidget(parent), settings(QSettings::IniFormat, QSettings::UserScope, "LYCEUM", "Bells-monitor")
{
    pTable[0] = nullptr;
    pTable[1] = nullptr;

    readSettings();

    m_pTcpSocket = new QTcpSocket(this);
    m_pTcpSocket->connectToHost(server_ip, quint16(server_port));

    connect(m_pTcpSocket,       SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket,       SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket,       SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)) );

    connect(&timerWait,         SIGNAL(timeout()), this, SLOT(slotTryReconnect())   );
    connect(&timerCurrentTime,  SIGNAL(timeout()), this, SLOT(slotSetCurrentTime()) );
    connect(&timerPing,         SIGNAL(timeout()), this, SLOT(slotPingOut()) );

    timerWait.start(timerWaitReconnect);

    pLayout = new QVBoxLayout;
    pLayout->setMargin(10);

    this->setStyleSheet("font-size: " + textSize + "px; gridline-color: black;");
    message.setStyleSheet("font-size: " + textSize + "px;");

    this->setLayout(pLayout);

    this->setCursor(QCursor(Qt::BlankCursor));

//Full Screen
    if( settings.value("settings/fullScreen").toBool() ){
        this->showFullScreen();
        this->setCursor(QCursor(Qt::BlankCursor));
    }
    else
        this->showMaximized();
}
void BellsMonitor::createClock(void)
{
    clockSetText();

    clock.setAlignment(Qt::AlignCenter);
//    clock.setStyleSheet("font: bold 48px;");
    timerCurrentTime.start(200);

 //Отображения смен по очедери

    pLayout->addWidget(&clock);
}
BellsMonitor::~BellsMonitor()
{

}
void BellsMonitor::errorServerConnection()
{
    message.setText(tr("Error server connection!"));
    message.setAlignment(Qt::AlignCenter);
    pLayout->addWidget(&message);
//    createClock();

    firstPartClock  = "\0";
    secondPartClock = "\0";

    isLessonNow[0] = false;
    isLessonNow[1] = false;
}
void BellsMonitor::createTable(int numbersOfLessons, short unsigned int numberOfTable)
{
    deleteTable(numberOfTable);

    if(numbersOfLessons == 0)
    {
        errorServerConnection();
        return;
    }

    int rows = numbersOfLessons + 2;

    pTable[numberOfTable] = new QTableWidget(rows,3);
    pTable[numberOfTable]->setStyleSheet("background-image: url(:/logo); \
                              background-repeat: repeat-xy; \
                              background-position: center; \
                              background-origin: content; background-attachment: scroll");

    pTable[numberOfTable]->setObjectName("table#"+QString::number(numberOfTable));
    eventFilter = new FingerSlide(pTable[numberOfTable]->viewport());
    pTable[numberOfTable]->viewport()->installEventFilter(eventFilter);

    connect(eventFilter, SIGNAL(signalRelease()), this, SLOT(slotDisplayPeriod()));
// ############################################################################################
//creating tables
    for (int column = 0; column < 3; column++)
        for (int row = 0; row < numbersOfLessons+2; row++){
            if(row == 0 && column == 0){
                pTable[numberOfTable]->setItem(row,column,new QTableWidgetItem);
                pTable[numberOfTable]->setSpan(0,0,1,3);
            }
            else if(row != 0){
                pTable[numberOfTable]->setItem(row,column,new QTableWidgetItem);
                pTable[numberOfTable]->item(row,column)->setTextAlignment(Qt::AlignCenter);
                pTable[numberOfTable]->item(row,column)->setBackgroundColor(QColor(backgroundColorAlpha));
            }
         }
//insert text in table
    for (int column = 0; column < 3; column++)
        for (int row = 2; row < numbersOfLessons+2; row++){
            if(column==1)
                pTable[numberOfTable]->item(row, column)->setText(pDoubleArray[numberOfTable][row-2].begin);
            if(column==2)
                pTable[numberOfTable]->item(row, column)->setText(pDoubleArray[numberOfTable][row-2].end);
        }
//set numbers for lessons
    for (int j = 2; j < rows; ++j)
        pTable[numberOfTable]->item(j, 0)->setText(QString::number(j-2));

//set number of period
    pTable[numberOfTable]->item(0, 0)->setBackgroundColor(QColor(250, 250, 250,200));
    pTable[numberOfTable]->item(0, 0)->setText("Смена №" + QString::number(numberOfTable+1));
    pTable[numberOfTable]->item(0, 0)->setTextColor(Qt::red);
    pTable[numberOfTable]->item(0, 0)->setTextAlignment(Qt::AlignCenter);

//set table's properties
    pTable[numberOfTable]->verticalHeader()->hide();
    pTable[numberOfTable]->horizontalHeader()->hide();
    pTable[numberOfTable]->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    pTable[numberOfTable]->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
//stretch
    for(int j = 0; j < rows; j++)
        pTable[numberOfTable]->verticalHeader()->setSectionResizeMode(j, QHeaderView::Stretch);
//properties
    pTable[numberOfTable]->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    pTable[numberOfTable]->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    pTable[numberOfTable]->setEditTriggers(0);
    pTable[numberOfTable]->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pTable[numberOfTable]->setSelectionMode(QAbstractItemView::NoSelection);
    pTable[numberOfTable]->setDragDropMode(QAbstractItemView::NoDragDrop);

    pTable[numberOfTable]->item(1, 0)->setBackgroundColor(headerBackgroundColor);
    pTable[numberOfTable]->item(1, 1)->setBackgroundColor(headerBackgroundColor);
    pTable[numberOfTable]->item(1, 2)->setBackgroundColor(headerBackgroundColor);

    pTable[numberOfTable]->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    pTable[numberOfTable]->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    zebra(numberOfTable);

//set text in headers
    pTable[numberOfTable]->item(1, 0)->setText("№");
    pTable[numberOfTable]->item(1, 1)->setText("Начало урока");
    pTable[numberOfTable]->item(1, 2)->setText("Окончание урока");
}
void BellsMonitor::clockSetText()
{
    if(showTime)
    {
        secondPartClock = secondPartClock + "<br>" \
                            + "<b>" \
                            + QTime::currentTime().toString("hh:mm:ss") \
                            + "   " \
                            + QDate::currentDate().toString("dd-MM-yyyy") \
                            + "<\b>";
    }
    clock.setText(firstPartClock + secondPartClock);
}
void BellsMonitor::slotPingOut()
{
    slotError(QAbstractSocket::SocketTimeoutError);
}
void BellsMonitor::slotReadyRead()
{
    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_5_3);


    int typeData;
    in >> typeData;
//    qDebug() << "typeData - " << typeData;
    if(typeData == 1)
    {
        timerPing.start(timerPingDuration);
        return;
    }
    else if (typeData == 0)
    {
        isChangesEnabled[0] = false;
        isChangesEnabled[1] = false;
        timerCurrentPeriodDisplay.stop();

        if(pDoubleArray != nullptr)
            for (int i = 0; i < 2; ++i)
                delete []pDoubleArray[i];

        pDoubleArray = new lessonTime* [2];

        for (int i = 0; i < 2; ++i) {
            in >> isChangesEnabled[i] \
               >> numbersOfLessonInChange[i];

            pDoubleArray[i] = new lessonTime[numbersOfLessonInChange[i]];

            for (int j = 0; j < numbersOfLessonInChange[i]; ++j){

                in >> pDoubleArray[i][j].begin >> pDoubleArray[i][j].end;

                QString begin   =  pDoubleArray[i][j].begin;
                QString end     =  pDoubleArray[i][j].end;

    //установка переменной nextLessonBeginInSec
                if( !(begin.startsWith(dash)) ) {

                    int hourBegin   = begin.split(":")[0].toInt();
                    int minutBegin  = begin.split(":")[1].toInt();

                    int hourEnd     = end.split(":")[0].toInt();
                    int minutEnd    = end.split(":")[1].toInt();

                    pDoubleArray[i][j].beginInSec = hourBegin * 3600 + minutBegin * 60;
                    pDoubleArray[i][j].endInSec = hourEnd * 3600 + minutEnd * 60;

                    if( j > 0 ){
                            for (int ii = j-1; ii >= 0; --ii) {
                                if(pDoubleArray[i][ii].begin.startsWith(dash))
                                    continue;
                                if(pDoubleArray[i][ii].nextLessonBeginInSec == -1){
                                    pDoubleArray[i][ii].nextLessonBeginInSec = pDoubleArray[i][j].beginInSec;
                                    break;
                                }
                            }
                    }
                }
            }
        }

    // nextLessonBeginInSec для последнего активного урока - это beginInSec первого активного урока
        clear();
        for (int i = 0; i < 2; ++i){
            for (int j = numbersOfLessonInChange[i]-1; j > 0; --j){
                if(pDoubleArray[i][j].begin.startsWith(dash))
                    continue;
                if(pDoubleArray[i][j].nextLessonBeginInSec == -1){
                    for (int ii = 0; ii < numbersOfLessonInChange[i]; ++ii) {
                        if(pDoubleArray[i][ii].begin.startsWith(dash))
                            continue;
                        pDoubleArray[i][j].nextLessonBeginInSec = pDoubleArray[i][ii].beginInSec;
                        break;
                    }
                }
                break;
            }

            createTable(numbersOfLessonInChange[i], unsigned(i));
            pTable[i]->hide();
            pLayout->addWidget(pTable[i]);
        }
        createClock();

        if(isChangesEnabled[0]){
            currentPeriodDisplay = 0;
            pTable[0]->show();
        }
        else if(isChangesEnabled[1]){
                currentPeriodDisplay = 1;
                pTable[1]->show();
            }
        else
            qDebug() << "error currentPeriodDisplay";

        disconnect(&timerCurrentPeriodDisplay, SIGNAL(timeout()), this, SLOT(slotDisplayPeriod()));


        if(isChangesEnabled[0] && isChangesEnabled[1]){
            connect(&timerCurrentPeriodDisplay,  SIGNAL(timeout()), this, SLOT(slotDisplayPeriod()) );
            timerCurrentPeriodDisplay.start(tableChangeTimer);
        }

        timerPing.start(timerPingDuration);
    }
    else
    {
        message.setText("Ошибка передачи данных");
        message.setAlignment(Qt::AlignCenter);
        pLayout->addWidget(&message);
    }
}
void BellsMonitor::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(m_pTcpSocket->errorString())
                    );
    qDebug() << strError;


    errorServerConnection();

    clear();

    m_pTcpSocket->disconnectFromHost();
    isConnected = false;
    timerWait.start(timerWaitReconnect);

    timerCurrentPeriodDisplay.stop();
    timerCurrentTime.stop();
    clock.clear();
}
void BellsMonitor::slotConnected()
{
    isConnected = true;
    timerWait.stop();
    qDebug() << "Received the connected() signal";
    message.clear();
}
void BellsMonitor::clear()
{
    for (int i = 0; i < 2; ++i){
        deleteTable(i);
        pLayout->removeWidget(&message);
    }
    pLayout->removeWidget(&clock);
}
void BellsMonitor::deleteTable(short unsigned int numberOfTable)
{
    if(pTable[numberOfTable] != nullptr){
        for (int column = 0; column < 3; column++){
            for (int row = 0; row < pTable[numberOfTable]->rowCount(); row++){
                delete pTable[numberOfTable]->item(row, column);
            }
        }
        if(eventFilter != nullptr)
        {
            pTable[numberOfTable]->viewport()->removeEventFilter(eventFilter);
            eventFilter = nullptr;
        }
        delete pTable[numberOfTable];
        pTable[numberOfTable] =nullptr;


    }
}
void BellsMonitor::slotTryReconnect()
{
//    qDebug() << "slot reconnect";
    timerWait.stop();
    m_pTcpSocket->connectToHost(server_ip, server_port);
}
void BellsMonitor::slotSetCurrentTime()
{
    if(currentPeriodDisplay < 0)
        return;

    if(isConnected){
        int currentTimeInSec = QTime::currentTime().hour() * 3600 \
                               + QTime::currentTime().minute() * 60 \
                               + QTime::currentTime().second();

        if(isLessonNow[currentPeriodDisplay]){
            firstPartClock = "Урок заканчивается через ";
            secondPartClock = restTime(pDoubleArray[currentPeriodDisplay][numberCurrentLesson[currentPeriodDisplay]].endInSec, currentTimeInSec);
            selectCurrentLesson(currentTimeInSec);
        }
        else if( !isLessonNow[currentPeriodDisplay]  && numberNextLesson[currentPeriodDisplay] != -1){
            firstPartClock = "Начало <span style='color:red'>" \
                    + QString::number(numberNextLesson[currentPeriodDisplay]) \
                    + QString("</span> урока через ");
            secondPartClock = restTime(pDoubleArray[currentPeriodDisplay][numberNextLesson[currentPeriodDisplay]].beginInSec , currentTimeInSec);
            selectCurrentLesson(currentTimeInSec);
        }
        else{
            firstPartClock.clear();
            secondPartClock.clear();
            selectCurrentLesson(currentTimeInSec);
        }
        secondPartClock = QString("<span style='color:red'>" + secondPartClock + "</span>");
    }

    clockSetText();


    int textSize_tmp = textSizeBuffer[currentPeriodDisplay];
    while( pTable[currentPeriodDisplay]->verticalScrollBar()->isVisible() && textSize_tmp > minTextSize) {
        --textSize_tmp;
        pTable[currentPeriodDisplay]->setStyleSheet("background-image: url(:/img/logo); \
                                                    background-repeat: repeat-xy; \
                                                    background-position: center; \
                                                    background-origin: content; background-attachment: scroll; \
                                                    font-size: " + QString::number(textSize_tmp) + "px;");
    }
    pTable[currentPeriodDisplay]->clearFocus();
}
void BellsMonitor::selectCurrentLesson(int currentTimeInSec)
{
    if(currentPeriodDisplay < 0)
        return;

        if(pTable[currentPeriodDisplay] != 0)
        {
            isLessonNow[currentPeriodDisplay] = false;
            numberCurrentLesson[currentPeriodDisplay] = -1;
            for (int j = 0; j < numbersOfLessonInChange[currentPeriodDisplay]; ++j) {
                if(pDoubleArray[currentPeriodDisplay][j].begin == "-- : --")
                    continue;

                if( (pDoubleArray[currentPeriodDisplay][j].beginInSec < currentTimeInSec) && (currentTimeInSec < pDoubleArray[currentPeriodDisplay][j].endInSec)){
                    pTable[currentPeriodDisplay]->item(j + 2, 0)->setTextColor(QColor(SelectTextColor));
                    pTable[currentPeriodDisplay]->item(j + 2, 1)->setTextColor(QColor(SelectTextColor));
                    pTable[currentPeriodDisplay]->item(j + 2, 2)->setTextColor(QColor(SelectTextColor));

                    isLessonNow[currentPeriodDisplay]         = true;
                    numberCurrentLesson[currentPeriodDisplay] = j;
                }
                else{
                    pTable[currentPeriodDisplay]->item(j + 2, 0)->setTextColor(QColor(textColor));
                    pTable[currentPeriodDisplay]->item(j + 2, 1)->setTextColor(QColor(textColor));
                    pTable[currentPeriodDisplay]->item(j + 2, 2)->setTextColor(QColor(textColor));
                }
            }
            if(!isLessonNow[currentPeriodDisplay]){
                numberNextLesson[currentPeriodDisplay] = -1;
                for (int j = 0; j < numbersOfLessonInChange[currentPeriodDisplay]; ++j){
                    if(pDoubleArray[currentPeriodDisplay][j].begin.startsWith(dash))
                        continue;
                    if(currentTimeInSec < pDoubleArray[currentPeriodDisplay][j].beginInSec){
                        numberNextLesson[currentPeriodDisplay] = j;
                        break;
                    }
                }
            }
            if(numberNextLesson[currentPeriodDisplay] == -1){
                for (int j = 0; j < numbersOfLessonInChange[currentPeriodDisplay]; ++j) {
                    if(pDoubleArray[currentPeriodDisplay][j].begin.startsWith(dash))
                        continue;
                    numberNextLesson[currentPeriodDisplay] = j;
                    break;
                }
            }
        }
}
void BellsMonitor::slotDisplayPeriod()
{
    timerCurrentPeriodDisplay.start(tableChangeTimer);

    if(isChangesEnabled[0] && isChangesEnabled[1])
    {
        if(currentPeriodDisplay == 0){
            currentPeriodDisplay = 1;
            pTable[0]->hide();
            pTable[1]->show();
        }
        else if(currentPeriodDisplay == 1){
            currentPeriodDisplay = 0;
            pTable[1]->hide();
            pTable[0]->show();
        }
        else
            return;
    }
}
QString BellsMonitor::restTime(int timeInSec, int currentTime)
{
    int result;
    int H, M, S;

    result = timeInSec - currentTime;
    if(result < 0)
        result += 86400;

    M = (result / 60);
    S = (result - (M * 60) );

    if(M > 59){
        H = M / 60;
        M = M - H * 60;
        return QString::number(H) + " час. " + QString::number(M) + " мин. " + QString::number(S) + " сек.";
    }

    return QString::number(M) + " мин. " + QString::number(S) + " сек.";
}
void BellsMonitor::zebra(short unsigned int numberOfTable)
{
    for (int j = 0; j < numbersOfLessonInChange[numberOfTable]; j += 2) {
        pTable[numberOfTable]->item(j + 2, 0)->setBackgroundColor(SelectBackgroundColor);
        pTable[numberOfTable]->item(j + 2, 1)->setBackgroundColor(SelectBackgroundColor);
        pTable[numberOfTable]->item(j + 2, 2)->setBackgroundColor(SelectBackgroundColor);
    }
}
bool BellsMonitor::event(QEvent *event)
{
//      qDebug() << "bells event" <<event->type();

    return QWidget::event(event);
}
void BellsMonitor::readSettings()
{
    QFile fileSettings(settings.fileName());

    if( !(fileSettings.exists()) ){
        settings.setValue("settings/server_ip",   "localhost");
        settings.setValue("settings/server_port",  8083);
        settings.setValue("settings/textSize",     48);
        settings.setValue("settings/minTextSize",   10);
        settings.setValue("settings/fullScreen",   false);
        settings.setValue("settings/tableChangeTimer",   15);
        settings.setValue("settings/showTime",   false);
    }
        server_ip       = settings.value("settings/server_ip",   "localhost").toString();
        server_port     = settings.value("settings/server_port", 8083).toInt();
        textSize        = settings.value("settings/textSize",    48).toString();
        minTextSize     = settings.value("settings/minTextSize",  10).toInt();
        fullScreen      = settings.value("settings/fullScreen",  false).toBool();
        tableChangeTimer= settings.value("settings/tableChangeTimer",  15).toInt();
        tableChangeTimer*=1000;
        showTime        = settings.value("settings/showTime", false).toBool();

    fileSettings.close();

    textSizeBuffer[0] = textSize.toInt();
    textSizeBuffer[1] = textSize.toInt();

    this->setStyleSheet("font: bold " + textSize +"px;");
}
