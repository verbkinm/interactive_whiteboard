#include "viewer.h"
#include "ui_viewer.h"
#include "myviewereventfilter.h"

#include <QDebug>
#include <QMessageBox>
#include <QScrollBar>



#define STEP        10
#define MAX_STEP    5
#define MARGIN      15

viewer::viewer(QString dirPath, QString textColor, unsigned int textSize, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::viewer)
{
    ui->setupUi(this);

    ui->next->setStyleSheet("font-size: " + QString::number(textSize) + "px; color:" + textColor + ";");
    ui->prevois->setStyleSheet("font-size: " + QString::number(textSize) + "px; color:" + textColor + ";");
    ui->real_size->setStyleSheet("font-size: " + QString::number(textSize) + "px; color:" + textColor + ";");
    ui->plus->setStyleSheet("font-size: " + QString::number(textSize) + "px; color:" + textColor + ";");
    ui->minus->setStyleSheet("font-size: " + QString::number(textSize) + "px; color:" + textColor + ";");

    ui->scrollArea->viewport()->installEventFilter(new MyViewerEventFilter(ui->scrollArea->viewport()));

    createImageList(dirPath);

    originPixmap  = QPixmap(list[it]);
    ui->label->setPixmap(originPixmap);

    connect(ui->plus,           SIGNAL(clicked(bool)), this, SLOT(slotPlusImage()));
    connect(ui->minus,          SIGNAL(clicked(bool)), this, SLOT(slotMinusImage()));
    connect(ui->real_size,      SIGNAL(clicked(bool)), this, SLOT(slotRealSize()));

    connect(ui->next,           SIGNAL(clicked(bool)), this, SLOT(slotNextImage()));
    connect(ui->prevois,        SIGNAL(clicked(bool)), this, SLOT(slotPrevoisImage()));

    ui->prevois->setEnabled(false);
}
void viewer::errorConfig(ERROR error)
{
    QMessageBox msgBox;

    switch (error) {
    case CANT_OPEN_DIR:{
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(QString("ОШИБКА!\n"
                               "Не возможно открыть каталог " + dir.path() + "!" ));
        qDebug() << "Не возможно открыть каталог " + dir.path() + "!";
        }
        break;
    case EMPTY_DIR:{
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(QString("ОШИБКА!\n"
                               "Каталог " + dir.path() + " не содержит файлов для отображения!" ));
        qDebug() << "Каталог " + dir.path() + " не содержит файлов для отображения!" ;
        }
        break;
    default:
        break;
    }

    msgBox.exec();
    exit(1);
}
void viewer::createImageList(QString dirPath)
{
    dir.setPath(dirPath);
    if(!dir.exists())
        errorConfig(CANT_OPEN_DIR);

    dir.setSorting(QDir::Name);
    QStringList filters;
    filters << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.gif" << "*.GIF";

    dir.setNameFilters(filters);

    QFileInfoList fl = dir.entryInfoList();
    foreach (QFileInfo fi, fl) {
        list << fi.filePath();
    }

    if(list.isEmpty())
        errorConfig(EMPTY_DIR);

}
void viewer::slotPlusImage()
{
    ui->minus->setEnabled(true);

    if( step >= MAX_STEP ){
        ui->plus->setEnabled(false);
        return;
    }
    ++step;

    width += (width / 100 * STEP);
    height += (height / 100 * STEP);

    ui->label->setPixmap(originPixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
void viewer::slotMinusImage()
{
    ui->plus->setEnabled(true);
    if(step <= -MAX_STEP ){
        ui->minus->setEnabled(false);
        return;
    }
    --step;

    width -= (width / 100 * STEP);
    height -= (height / 100 * STEP);
    ui->label->setPixmap(originPixmap.scaled(width, height, Qt::KeepAspectRatio));
}
void viewer::slotRealSize()
{
    ui->label->setPixmap(originPixmap.scaled(ui->scrollArea->width() - ui->scrollArea->frameWidth() - MARGIN, \
                                              ui->scrollArea->height() - ui->scrollArea->frameWidth() - MARGIN, \
                                              Qt::KeepAspectRatio, Qt::SmoothTransformation));
    width = ui->label->pixmap()->width();
    height = ui->label->pixmap()->height();

    step = 0;

    ui->plus->setEnabled(true);
    ui->minus->setEnabled(true);
}
void viewer::slotNextImage()
{
    ui->prevois->setEnabled(true);
    if(it != list.length() - 1){
        qDebug() << list.length();
        originPixmap  = QPixmap(list.at(++it));
        ui->label->setPixmap(originPixmap);
        slotRealSize();
    }
    if(it == list.length() - 1)
        ui->next->setEnabled(false);
}
void viewer::slotPrevoisImage()
{
    ui->next->setEnabled(true);
    if(it != 0){
        originPixmap  = QPixmap(list.at(--it));
        ui->label->setPixmap(originPixmap);
        slotRealSize();
    }
    if(it == 0)
        ui->prevois->setEnabled(false);
}
bool viewer::event(QEvent *event)
{
    if(event->type() == QEvent::Resize){
        ui->label->setPixmap(originPixmap.scaled(ui->scrollArea->width() - ui->scrollArea->frameWidth() - MARGIN, \
                                                 ui->scrollArea->height() - ui->scrollArea->frameWidth() - MARGIN, \
                                                 Qt::KeepAspectRatio, Qt::SmoothTransformation));
        width = ui->label->pixmap()->width();
        height = ui->label->pixmap()->height();
        step = 0;

//        qDebug() << ui->scrollArea->
    }

    return QWidget::event(event);
}
viewer::~viewer()
{
    delete ui;
}
