#include "wrapper.h"

Wrapper::Wrapper(QObject *parent) : QObject(parent)
{
    w = new Main_Widget;
    w->showFullScreen();

    this->addPath(w->widget_settings.fileName());
    this->addPath(w->generals_settings.fileName());

    connect(&watcher, SIGNAL(fileChanged(QString)), SLOT(slotFileChange()) );
}

void Wrapper::addPath(QString path)
{
    watcher.addPath(path);
}

void Wrapper::slotFileChange()
{
    qDebug() << "setting changed";
    delete w;
    w = new Main_Widget;
    w->showFullScreen();

    this->addPath(w->widget_settings.fileName());
    this->addPath(w->generals_settings.fileName());
}
