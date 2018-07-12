#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QPixmap>
#include <QDir>
#include <QFileInfoList>

#include "myviewereventfilter.h"


namespace Ui {
class viewer;
}

class viewer : public QWidget
{
    Q_OBJECT

public:
    viewer(QString dirPath, QString textColor, unsigned int textSize, QWidget *parent = 0);
    ~viewer();


private:
    Ui::viewer *ui;

    QPixmap     originPixmap;
    float       width   = 0;
    float       height  = 0;

// текущий шаг при масштабировании
    int         step    = 0;

    QDir dir;
    QStringList list;
// итератор при листании слайдов
    int it     = 0;

//координаты курсора
    int x, y;

    MyViewerEventFilter* eventFilter = nullptr;

    virtual bool event(QEvent *event);

    enum ERROR {CANT_OPEN_DIR, EMPTY_DIR, SYNTAX_ERROR};

    void errorConfig(ERROR error);

    void createImageList(QString dirPath);

    void setPageNumbers();

private slots:
    void slotPlusImage();
    void slotMinusImage();
    void slotRealSize();

    void slotNextImage();
    void slotPrevoisImage();

signals:
// сигнал передается в класс Content - прерывание таймера бездействия
    void    signalTimerStart();
};

#endif // VIEWER_H