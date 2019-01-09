#ifndef WIDGETFORMINIWIDGET_H
#define WIDGETFORMINIWIDGET_H

#include "../structes/structes.h"
#include "../widget_types/leafer.h"

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QEvent>
#include <QDir>
#include <QTimer>
#include <QMap>

class WidgetForMiniWidget : public QWidget
{
    Q_OBJECT

public:
    WidgetForMiniWidget(settingsMiniWidget *struct_settingsMiniWidget, \
                        QWidget *parent = nullptr);
    ~WidgetForMiniWidget();

    int getCurrentPage();

    void            dynamicWidget(QString dirPath, bool state);

    void            create_or_recreate_object(settingsMiniWidget *struct_settingsMiniWidget);

    void            addMainWidget(QWidget* widget);

    void            setTitleText(QString text);

    void            setTitleStyle(QMap<QString, QVariant> map);
    void            setStyleMainWidget(QMap<QString, QVariant> map);
    void            setBackgroundMainWidget(QString background);
    void            setPatternMainWidget(QString pattern);
    void            setSpeedMainWidget(int speed);

    void            setIconPath(QString path);
    void            setDirPath (QString path);
    void            setText (QString text);
    void            setTimerInterval(uint interval);

private:
    settingsMiniWidget *pStruct_settingsMiniWidget = nullptr;

    QLabel*         title       = nullptr;
    QLabel*         image       = nullptr;

    QVBoxLayout*    layout      = nullptr;

    Leafer*         leafer      = nullptr;

    QPixmap         currentPix;

    QString         dirPath;
    QDir            dir;
    QStringList     list;

    //указатель на отображаемый виджет, если тип мини виджета такой как (clock, date, run_string и т.д)
    QWidget*        mainWidget  = nullptr;

// таймер для динамического виджета(где переключаются картинки)
    QTimer          timer;
// итератор при листании слайдов
    int             it          = 0;
//если файл рисунка существует - возвращает его путь, если нет - файл рисунка по умолчание(logo)
    QString         correct_image       (QString path);

    void            setImage            ();
    void            createImageList     (QString dirPath);

    virtual bool    event               (QEvent *event);

private slots:
    void slotPrevios();
    void slotNext();

signals:
    void signalImagePressed();
    void signalImageReleased();
    void signalImageClicked();
};

#endif // WIDGETFORMINIWIDGET_H
