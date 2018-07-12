#ifndef CELL_H
#define CELL_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QGridLayout>
#include <QLabel>


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

    bool bold  = false;

    unsigned int textSize;
    QString textColor;

    QLabel* pLabel = nullptr;

public:
    Cell(unsigned int textSize, QString textColor, bool bold = false);
    ~Cell();

    enum tp {NONE, NUMBER, TIME, LESSON, HEADER} TYPE_COLUMN;
    bool background = false;

    void        setText                 (QString str);
    void        separate                ();
    void        setBackgroundColor      (tp role);

    void        paintEvent              (QPaintEvent*);
signals:

public slots:
};

#endif // CELL_H
