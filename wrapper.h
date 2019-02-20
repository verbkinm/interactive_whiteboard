#ifndef WRAPPER_H
#define WRAPPER_H

#include "main_widget/main_widget.h"

#include <QObject>
#include <QFileSystemWatcher>
#include <QDebug>

class Wrapper : public QObject
{
    Q_OBJECT
public:
    Wrapper(QObject *parent = nullptr);

private:
    Main_Widget* w = nullptr;
    QFileSystemWatcher watcher;

    void addPath(QString path);

signals:

public slots:
    void slotFileChange();
};

#endif // WRAPPER_H
