#ifndef WIDGETFORMINIWIDGET_H
#define WIDGETFORMINIWIDGET_H

#include "../structes/structes.h"
#include "../widget_types/leafer.h"

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QEvent>

class WidgetForMiniWidget : public QWidget
{
    Q_OBJECT

public:
    WidgetForMiniWidget(const path *struct_path, \
                        const text *struct_text, \
                        const miscellanea *struct_miscellanea,
                        QSize *size, \
                        QWidget *parent = 0);
    ~WidgetForMiniWidget();

    QLabel* getLabel();

private:

    QLabel*         title       = nullptr;
    QLabel*         image       = nullptr;

    QPushButton*    previos     = nullptr;
    QPushButton*    next        = nullptr;

    QLabel*         count       = nullptr;

    QVBoxLayout*    layout      = nullptr;

    QString         dirPath;

    Leafer*         leafer      = nullptr;

    void         setImage           ();

    virtual bool event              (QEvent *event);

private slots:
    void slotPrevios();
    void slotNext();

signals:
    void signalImagePressed();
    void signalImageReleased();
    void signalImageClicked();
};

#endif // WIDGETFORMINIWIDGET_H
