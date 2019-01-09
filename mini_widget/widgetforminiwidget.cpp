#include "widgetforminiwidget.h"

#include <QDebug>
#include <QPainter>
#include <QMouseEvent>

#include "widget_types/clock.h"
#include "widget_types/date.h"
#include "widget_types/run_string.h"

WidgetForMiniWidget::WidgetForMiniWidget(settingsMiniWidget *struct_settingsMiniWidget, \
                                          QWidget *parent) : QWidget(parent)
{
    create_or_recreate_object(struct_settingsMiniWidget);
}
void WidgetForMiniWidget::create_or_recreate_object(settingsMiniWidget *struct_settingsMiniWidget)
{
    pStruct_settingsMiniWidget = struct_settingsMiniWidget;
    this->resize(pStruct_settingsMiniWidget->size);

    if(layout == nullptr){
        layout      = new QVBoxLayout;
        layout->setMargin(0);
    }

    if(title == nullptr)
        title       = new QLabel("Title");

    if(image == nullptr)
        image       = new QLabel("Image");

    if(leafer == nullptr)
        leafer      = new Leafer(pStruct_settingsMiniWidget->text.titleTextSize);
    else
        leafer->setTextSize(pStruct_settingsMiniWidget->text.titleTextSize);

    layout->addWidget(title, 0, Qt::AlignHCenter);
    layout->addWidget(image, 2);
    layout->addWidget(leafer, 0, Qt::AlignHCenter);

    title->setText(pStruct_settingsMiniWidget->text.titleText);
    title->setStyleSheet("font-size:" + QString::number(pStruct_settingsMiniWidget->text.titleTextSize) + \
                         "px; color: " + pStruct_settingsMiniWidget->text.titleColor);
    leafer->setStyleSheet("font-size:" + QString::number(pStruct_settingsMiniWidget->text.titleTextSize) + \
                         "px; color: " + pStruct_settingsMiniWidget->text.titleColor);

    this->setLayout(layout);

    disconnect(leafer, SIGNAL(signalPrevios()), this, SLOT(slotPrevios()));
    disconnect(leafer, SIGNAL(signalNext()), this, SLOT(slotNext()));
    connect(leafer, SIGNAL(signalPrevios()), this, SLOT(slotPrevios()));
    connect(leafer, SIGNAL(signalNext()), this, SLOT(slotNext()));

    title->hide();
    leafer->hide();

    setIconPath(pStruct_settingsMiniWidget->path.iconPath);

    timer.stop();
    timer.setInterval(pStruct_settingsMiniWidget->miscellanea.dynamicMiniWidgetTimer*1000);

    dynamicWidget(pStruct_settingsMiniWidget->path.dirPath, pStruct_settingsMiniWidget->miscellanea.dynamicMiniWidget);
}

void WidgetForMiniWidget::addMainWidget(QWidget *widget)
{
    image->hide();
    layout->addWidget(widget);
    mainWidget = widget;
}

void WidgetForMiniWidget::setTitleText(QString text)
{
    title->setText(text);
}

void WidgetForMiniWidget::setTitleStyle(QMap<QString, QVariant> map)
{
    title->setStyleSheet("font-size:" + map["textSize"].toString() + "px; " + \
                          "color: " + map["textColor"].toString());

    leafer->setTextSize(map["textSize"].toInt());
    leafer->setStyleSheet("color: " + map["textColor"].toString());
}

void WidgetForMiniWidget::setStyleMainWidget(QMap<QString, QVariant> map)
{
//    if(mainWidget != nullptr){
//        Clock* pClock           = nullptr;
//        Date* pDate             = nullptr;
//        Run_String* runString   = nullptr;

//        switch (pStruct_settingsMiniWidget->pGetType(mainWidget->objectName()))
//        {
//            case CLOCK:
//                pClock = static_cast<Clock*>(mainWidget);
//                pClock->setTextStyle(map);
//                break;
//            case DATE:
//                pDate = static_cast<Date*>(mainWidget);
//                pDate->setTextStyle(map);
//                break;
//            case RUN_STRING:
//                runString = static_cast<Run_String*>(mainWidget);
//                runString->setTextStyle(map);
//                break;
//        }
//    }
}

void WidgetForMiniWidget::setBackgroundMainWidget(QString background)
{
//    if(mainWidget != nullptr){
//        Clock* pClock           = nullptr;
//        Date* pDate             = nullptr;
//        Run_String* runString   = nullptr;

//        switch (pStruct_settingsMiniWidget->pGetType(mainWidget->objectName()))
//        {
//            case CLOCK:
//                pClock = static_cast<Clock*>(mainWidget);
//                pClock->setBackgroundStyle(background);
//                break;
//            case DATE:
//                pDate = static_cast<Date*>(mainWidget);
//                pDate->setBackgroundStyle(background);
//                break;
//            case RUN_STRING:
//                runString = static_cast<Run_String*>(mainWidget);
//                runString->setBackgroundStyle(background);
//                break;
//        }
//    }
}

void WidgetForMiniWidget::setPatternMainWidget(QString pattern)
{
//    if(mainWidget != nullptr){
//        Date* pDate             = nullptr;

//        switch (pStruct_settingsMiniWidget->pGetType(mainWidget->objectName()))
//        {
//            case DATE:
//                pDate = static_cast<Date*>(mainWidget);
//                pDate->setPattern(pattern);
//                break;
//        }
//    }
}

void WidgetForMiniWidget::setSpeedMainWidget(int speed)
{
//    if(mainWidget != nullptr){
//        Run_String* runString   = nullptr;

//        switch (pStruct_settingsMiniWidget->pGetType(mainWidget->objectName()))
//        {
//            case RUN_STRING:
//                runString = static_cast<Run_String*>(mainWidget);
//                runString->setSpeed(speed);
//                break;
//        }
//    }
}

void WidgetForMiniWidget::setIconPath(QString path)
{
    currentPix.load(correct_image(path));
    setImage();
}

void WidgetForMiniWidget::setDirPath(QString path)
{
    dynamicWidget(path, true);
    //    slotNext();
}

void WidgetForMiniWidget::setText(QString text)
{
    if(mainWidget != nullptr){
        Run_String* runString   = nullptr;

        switch (pStruct_settingsMiniWidget->pGetType(mainWidget->objectName()))
        {
            case RUN_STRING:
                runString = static_cast<Run_String*>(mainWidget);
                runString->setText(text);
                break;
        }
    }
}

void WidgetForMiniWidget::setTimerInterval(uint interval)
{
    timer.setInterval(int(interval) * 1000);
}
void WidgetForMiniWidget::dynamicWidget(QString dirPath, bool state)
{
    if(state)
    {
        title->setVisible(true);
        leafer->setVisible(true);

        if(!dirPath.isEmpty()){
            this->dirPath = dirPath;
            createImageList(this->dirPath);
        }

        if(!list.isEmpty())
        {
            currentPix.load( correct_image(list.at(0)) );
            setImage();
            it = 0;
            leafer->setCounter(it+1, list.length());

            disconnect(&timer, SIGNAL(timeout()), this, SLOT(slotNext()));
            connect(&timer, SIGNAL(timeout()), this, SLOT(slotNext()));
            timer.start();
        }
    }
    else{
        title->setVisible(false);
        leafer->setVisible(false);
        timer.stop();
        setIconPath(pStruct_settingsMiniWidget->path.iconPath);
        it = 0;
        //нужно если включать dynamicWidget нужно для виджета типа label
        if(!dirPath.isEmpty()){
            this->dirPath = dirPath;
            createImageList(this->dirPath);
        }
    }
}
void WidgetForMiniWidget::createImageList(QString dirPath)
{
    dir.setPath(dirPath);

    dir.setSorting(QDir::Name);
    QStringList filters;
    filters << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.gif" << "*.GIF";

    dir.setNameFilters(filters);

    QFileInfoList fl = dir.entryInfoList();

    list.clear();
    foreach (QFileInfo fi, fl)
        list << fi.filePath();

    leafer->setCounter(it+1, list.length());
}
void WidgetForMiniWidget::slotPrevios()
{
    timer.stop();

    createImageList(dirPath);

    if(!list.isEmpty())
    {
        --it;

        if(it > -1)
            currentPix  = QPixmap(list.at(it));
        if(it < 0){
            currentPix  = QPixmap(list.at(list.length()-1));
            it = list.length()-1;
        }
        setImage();
        leafer->setCounter(it+1, list.length());
    }

    timer.start();
}
void WidgetForMiniWidget::slotNext()
{
    timer.stop();

    createImageList(dirPath);

    if(!list.isEmpty())
    {
        ++it;

        if(it < list.length())
            currentPix  = QPixmap(list.at(it));
        if(it >= list.length()){
            currentPix  = QPixmap(list.at(0));
            it = 0;
        }

        setImage();
        leafer->setCounter(it+1, list.length());
    }

    timer.start();
}
int WidgetForMiniWidget::getCurrentPage()
{
    return it;
}
void WidgetForMiniWidget::setImage()
{
    QPixmap newPixmap = currentPix.scaled(image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    image->setPixmap(newPixmap);
    image->setStyleSheet("QLabel { background-color : rgba(255, 255, 255, 30%);}");
    image->setAlignment(Qt::AlignCenter);
}
QString WidgetForMiniWidget::correct_image(QString path)
{
    QFile file(path);
    if(file.exists() && file.open(QIODevice::ReadOnly)){
        file.close();
        return path;
    }

    return QString(":/img/logo");
}
bool WidgetForMiniWidget::event(QEvent *event)
{
//    qDebug() << event->type();
    static bool cursorOnWidget;

    // при нажатии на виджет (а точнее на виджет изображения - image) появляется рамка, при ударживании кнопки мыши и вывыедения курсора за границы виджета рамка исчезает,
    // а при возвращении обратно (при удержании кнопки мыши) - рамка появляется
    if(event->type() == QEvent::MouseMove){
        if( image->geometry().contains(this->mapFromGlobal(QCursor::pos())) ){
            emit signalImagePressed();
            cursorOnWidget = true;
        }
        else {
            emit signalImageReleased();
            cursorOnWidget = false;
        }
    }
    if(event->type() == QEvent::MouseButtonPress){
        if(image->underMouse())
        {
            emit signalImagePressed();
            cursorOnWidget = true;
        }
    }
    // при отпускании кнопки проверяется, где в данный момент находится курсор - над виджетом или нет.
    // Если над виджетом - выполняются последующие действия
    if(event->type() == QEvent::MouseButtonRelease){
        emit signalImageReleased();
        if(cursorOnWidget)
            emit signalImageClicked();
    }

    if(event->type() == QEvent::Resize)
        setImage();

    return QWidget::event(event);
}

WidgetForMiniWidget::~WidgetForMiniWidget()
{
    delete title;
    delete image;
    delete leafer;
    delete layout;
}
