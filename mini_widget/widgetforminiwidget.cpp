#include "widgetforminiwidget.h"

#include <QDebug>
#include <QPainter>
#include <QMouseEvent>

WidgetForMiniWidget::WidgetForMiniWidget(const struct path *struct_path, \
                                         const struct text *struct_text, \
                                         const struct miscellanea *struct_miscellanea, \
                                         QSize *size, \
                                          QWidget *parent) :
    QWidget(parent)
{

    this->setFixedSize(*size);

    layout      = new QVBoxLayout;

    title       = new QLabel("Title");
    title->setStyleSheet("font-size:" + QString::number(struct_text->textSize) + "px;");

    image       = new QLabel("Image");


    leafer      = new Leafer(struct_text->textSize);


    layout->addWidget(title, 0, Qt::AlignHCenter);
    layout->addWidget(image, 2);
    layout->addWidget(leafer, 0, Qt::AlignHCenter);

    title->setText(struct_text->titleText);

    this->setLayout(layout);

    dirPath = struct_path->iconPath;

    setImage();

    if(!struct_miscellanea->dynamicMiniWidget)
    {
        title->hide();
        leafer->hide();
    }

    connect(leafer, SIGNAL(signalPrevios()), this, SLOT(slotPrevios()));
    connect(leafer, SIGNAL(signalNext()), this, SLOT(slotNext()));
}
void WidgetForMiniWidget::slotPrevios()
{
//    qDebug() << "previos";
}
void WidgetForMiniWidget::slotNext()
{
//    qDebug() << "next";
}
QLabel* WidgetForMiniWidget::getLabel()
{
    return image;
}
void WidgetForMiniWidget::setImage()
{
    QPixmap pixMap(dirPath);
    QPixmap newPixmap = pixMap.scaled(image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    image->setPixmap(newPixmap);
    image->setStyleSheet("QLabel { background-color : rgba(255, 255, 255, 30%); color : blue; }");
    image->setAlignment(Qt::AlignCenter);
}
bool WidgetForMiniWidget::event(QEvent *event)
{
//    qDebug() << event->type();
    static bool cursorOnWidget;

    // при нажатии на виджет появляется рамка, при ударживании кнопки нажатия и вывыедения курсора за границы виджета рамка исчезает,
    // а при возвращении обратно - рамка появляется
    if(event->type() == QEvent::MouseMove){
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->x() < 0 || mouseEvent->y() < 0 || mouseEvent->x() > this->width() || mouseEvent->y() > this->height()){
            emit signalImageReleased();
            cursorOnWidget = false;
        }
        else if(mouseEvent->x() > 0 && mouseEvent->y() > 0 && mouseEvent->x() < this->width() && mouseEvent->y() < this->height()){
            emit signalImagePressed();
            cursorOnWidget = true;
        }
    }
    if(event->type() == QEvent::MouseButtonPress){
        emit signalImagePressed();
        cursorOnWidget = true;
    }
    // при отпускании кнопки проверяется, где в данный момент находится курсор - над виджетом или нет.
    // Если над виджетом - выполняются последующие действия
    if(event->type() == QEvent::MouseButtonRelease)
        if(cursorOnWidget)
            emit signalImageClicked();


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
