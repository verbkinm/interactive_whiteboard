#ifndef CELL_H
#define CELL_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QGridLayout>


class Cell : public QWidget
{
    Q_OBJECT
private:
    QHBoxLayout* pHorizontLayout = nullptr;
    QVBoxLayout pVerticalLayoutArray;

    QGridLayout* pGridLayout = nullptr;

// номер текущего столбца и строки в gridLayout
    int row = 0;
    int column = 0;

    unsigned int textSize;
    QString textColor;

public:
    Cell(unsigned int textSize, QString textColor);

    void        setText                 (QString str);

    void        separate                ();
signals:

public slots:
};

#endif // CELL_H
