#include "viewer.h"
#include "ui_viewer.h"

#include <QDebug>


#define STEP 20

viewer::viewer(QString dirPath, QString textColor, unsigned int textSize, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::viewer)
{
    ui->setupUi(this);

    originPixmap  = QPixmap("/home/verbkinm/myProg/QT/interactive_whiteboard/img/school.jpg");
    width = originPixmap.size().width();
    height= originPixmap.size().height();

    ui->label->setPixmap(originPixmap);

    connect(ui->plus,           SIGNAL(clicked(bool)), this, SLOT(slotPlusImage()));
    connect(ui->minus,          SIGNAL(clicked(bool)), this, SLOT(slotMinusImage()));
    connect(ui->real_size,      SIGNAL(clicked(bool)), this, SLOT(slotRealSize()));

    qDebug() << ui->scrollArea->size();
}
void viewer::slotPlusImage()
{
    width += (width / 100 * STEP);
    height += (height / 100 * STEP);

    ui->label->setPixmap(originPixmap.scaled(width, height, Qt::KeepAspectRatio));
}
void viewer::slotMinusImage()
{
    width -= (width / 100 * STEP);
    height -= (height / 100 * STEP);

    ui->label->setPixmap(originPixmap.scaled(width, height, Qt::KeepAspectRatio));
}
void viewer::slotRealSize()
{
    width = originPixmap.size().width();
    height= originPixmap.size().height();
    ui->label->setPixmap(originPixmap);
}
viewer::~viewer()
{
    delete ui;
}
