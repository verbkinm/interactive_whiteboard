#ifndef LEAFER_H
#define LEAFER_H

#include <QWidget>

namespace Ui {
class Leafer;
}

class Leafer : public QWidget
{
    Q_OBJECT

public:
    Leafer(int textSize, QWidget *parent = nullptr);
    ~Leafer();

    void setCounter(int currentPosition, int totalPages);
    void setTextSize(int textSize);

private:
    Ui::Leafer *ui;

    int buttonSize;

signals:
    void signalPrevios();
    void signalNext();

private slots:
    void slotPreviosPush();
    void slotNextPush();

    void slotPreviosRelease();
    void slotNextRelease();

};

#endif // LEAFER_H
