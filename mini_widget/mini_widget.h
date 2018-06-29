#ifndef MINI_WIDGET_H
#define MINI_WIDGET_H

#include "widget_types/schedule.h"
#include "widget_types/myWidgets/viewer/viewer.h"

#include <QWidget>
#include <QLabel>
#include <QSize>

#include "content/content.h"


class Mini_Widget : public QWidget
{
    Q_OBJECT
public:

    Mini_Widget(QString borderColor, int borderWidth, QString borderClickColor, int borderClickWidth, \
                QPixmap* miniIcon, QSize size, \
                QString xmlPath,   QString dirPath, \
                QString textColor, QString backgroundColor, \
                QString titleText, unsigned int textSize, QString type, QWidget *parent);

void createLabelWidget(QPixmap* miniIcon, QSize size);

void createClockWidget(QSize size, QString textColor, QString backgroudColor);

void createScheduleWidget(QPixmap* miniIcon, QSize size, \
                          QString xmlPath,   QString textColor, \
                          unsigned int textSize, QString titleText);
//type image_viewer
//    Mini_Widget(QString borderColor, int borderWidth, QString borderClickColor, int borderClickWidth, \
//                QPixmap* miniIcon, QSize size, \
//                QString dirPath,   QString textColor, \
//                QString titleText, unsigned int textSize, QWidget *parent);
void createImageViewerWidget(QPixmap* miniIcon, QSize size, \
                             QString dirPath,   QString textColor, \
                             QString titleText, unsigned int textSize);
    //~Mini_Widget();

private:

    enum TYPE_WIDGETS{LABEL, CLOCK, SCHEDULE, IMAGE_VIEWER};
//рамка
    QLabel*                 border;
//рамка, которая будет появлятся при нажатии
    QLabel*                 borderClick;
    int                     borderClickWidth;
//центральная миниатюра
    QPixmap*                miniIcon;
    QLabel*                 centralLabel;
// центральный виджет в таком типе виджета как clock
    QWidget*                centralWidget;
//указатель на содержимое
    Content*                pContent = 0;

//сохранение настроек
    QString                 *xmlPath, *textColor, \
                            *dirPath, *titleText;

    unsigned int            *type;
    unsigned int            *textSize;

    Schedule* pSchedule     =   nullptr;
    viewer*   pImageViewer  =   nullptr;
//FUNCTIONS

    void    paintEvent              (QPaintEvent*);
    virtual bool event              (QEvent *event);

// для установления общих параметров для всех виджетов
    void    generalSettings         (QString borderColor, int borderWidth, QString borderClickColor, int borderClickWidth, QSize size);
// просто создание мини виджета с миниатюрой
    void    createLabelForMiniWidget(QPixmap* miniIcon, QSize size);

    void    setTypeValue            (QString typeStr);
signals:

public slots:

private slots:
// удаление объекта из памяти при закрытии окна
    void slotDeleteWidgetInContent();
};

#endif // MINI_WIDGET_H
