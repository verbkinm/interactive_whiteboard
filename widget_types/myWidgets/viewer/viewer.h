#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QPixmap>
#include <QDir>
#include <QFileInfoList>



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
    float       width, height;

    int         step;

    QDir dir;
    QStringList list;
    unsigned int it = 0;

//координаты курсора
    int x, y;

    virtual bool event(QEvent *event);

    enum ERROR {CANT_OPEN_DIR, EMPTY_DIR, SYNTAX_ERROR};

    void errorConfig(ERROR error);

    void createImageList(QString dirPath);

private slots:
    void slotPlusImage();
    void slotMinusImage();
    void slotRealSize();

    void slotNextImage();
    void slotPrevoisImage();
};

#endif // VIEWER_H
