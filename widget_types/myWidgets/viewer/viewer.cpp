#include "viewer.h"
#include "ui_viewer.h"

#include <QDebug>
#include <QMessageBox>
#include <QScrollBar>

#define STEP        10
#define MAX_STEP    5
#define MARGIN      15

viewer::viewer(QString dirPath, QString textColor, unsigned int textSize, int startPage, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::viewer)
{
    ui->setupUi(this);

    ui->next->setStyleSheet("font-size: " + QString::number(textSize) + "px; color:" + textColor + ";");
    ui->next->setIconSize(QSize(textSize*2,textSize*2));
    ui->prevois->setStyleSheet("font-size: " + QString::number(textSize) + "px; color:" + textColor + ";");
    ui->prevois->setIconSize(QSize(textSize*2,textSize*2));
    ui->real_size->setStyleSheet("font-size: " + QString::number(textSize) + "px; color:" + textColor + ";");
    ui->plus->setStyleSheet("font-size: " + QString::number(textSize) + "px; color:" + textColor + ";");
    ui->minus->setStyleSheet("font-size: " + QString::number(textSize) + "px; color:" + textColor + ";");
    ui->pages->setStyleSheet("font-size: " + QString::number(textSize) + "px; color:" + textColor + ";");

    eventFilter = new FingerSlide(ui->scrollArea->viewport());
    ui->scrollArea->viewport()->installEventFilter(eventFilter);

    this->dirPath = dirPath;
    it = startPage;

    createImageList(dirPath);

    if(!CRITICAL_ERROR && it < list.length() && it > -1)
        originPixmap  = QPixmap(list[it]);

    ui->label->setPixmap(originPixmap);


    connect(ui->plus,           SIGNAL(clicked(bool)), this, SLOT(slotPlusImage()));
    connect(ui->minus,          SIGNAL(clicked(bool)), this, SLOT(slotMinusImage()));
    connect(ui->real_size,      SIGNAL(clicked(bool)), this, SLOT(slotRealSize()));

    connect(ui->next,           SIGNAL(clicked(bool)), this, SLOT(slotNextImage()));
    connect(ui->prevois,        SIGNAL(clicked(bool)), this, SLOT(slotPrevoisImage()));
}
void viewer::errorConfig(ERROR error)
{
    CRITICAL_ERROR = true;

    QMessageBox msgBox;

    switch (error) {
    case CANT_OPEN_DIR:{
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(QString("ОШИБКА!\n"
                               "Не возможно открыть каталог \"" + dir.path() + "\" !" ));
        qDebug() << "Не возможно открыть каталог " + dir.path() + "!";
        }
        break;
    case EMPTY_DIR:{
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(QString("ОШИБКА!\n"
                               "Каталог \"" + dir.path() + "\" не содержит файлов для отображения!" ));
        qDebug() << "Каталог " + dir.path() + " не содержит файлов для отображения!" ;
        }
        break;
    default:
        break;
    }
    msgBox.exec();
//    exit(1);
}
void viewer::createImageList(QString dirPath)
{
    dir.setPath(dirPath);
    if(!dir.exists()){
        errorConfig(CANT_OPEN_DIR);
        return;
    }

    dir.setSorting(QDir::Name);
    QStringList filters;
    filters << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.gif" << "*.GIF";

    dir.setNameFilters(filters);

    QFileInfoList fl = dir.entryInfoList();

    list.clear();
    foreach (QFileInfo fi, fl)
        list << fi.filePath();

    if(list.isEmpty()){
        errorConfig(EMPTY_DIR);
        return;
    }

    setPageNumbers();

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
    createImageList(dirPath);

    if(!list.isEmpty())
    {
        ++it;

        if(it < list.length()){
            originPixmap  = QPixmap(list.at(it));
            ui->label->setPixmap(originPixmap);
        }
        if(it >= list.length()){
            originPixmap  = QPixmap(list.at(0));
            ui->label->setPixmap(originPixmap);
            it = 0;
        }
    }

    slotRealSize();
    setPageNumbers();
}
void viewer::slotPrevoisImage()
{
    createImageList(dirPath);

    if(!list.isEmpty())
    {
        --it;

        if(it > -1)
        {
            originPixmap  = QPixmap(list.at(it));
            ui->label->setPixmap(originPixmap);
        }
        if(it < 0){
            originPixmap  = QPixmap(list.at(list.length()-1));
            it = list.length()-1;
        }
    }

    slotRealSize();
    setPageNumbers();
}
bool viewer::event(QEvent *event)
{
    emit signalTimerStart();

    if(event->type() == QEvent::Resize){
        ui->label->setPixmap(originPixmap.scaled(ui->scrollArea->width() - ui->scrollArea->frameWidth() - MARGIN, \
                                                 ui->scrollArea->height() - ui->scrollArea->frameWidth() - MARGIN, \
                                                 Qt::KeepAspectRatio, Qt::SmoothTransformation));
        width = ui->label->pixmap()->width();
        height = ui->label->pixmap()->height();
        step = 0;
    }

    return QWidget::event(event);
}
void viewer::setPageNumbers()
{
    QString firstPart, secondPart;
    firstPart = QString::number(it + 1);
    secondPart= QString::number(list.length());
    ui->pages->setText(firstPart + " / " + secondPart);
    this->setFocus();
}
viewer::~viewer()
{
//    qDebug() << "viewer destructor";
    delete ui;
    delete eventFilter;
}
