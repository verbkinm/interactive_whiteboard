#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QPixmap>


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

    QPixmap originPixmap;
    float width, height;

private slots:
    void slotPlusImage();
    void slotMinusImage();
    void slotRealSize();
};

#endif // VIEWER_H
