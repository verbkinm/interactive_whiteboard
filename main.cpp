#include "main_widget/main_widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    a.setOverrideCursor(Qt::BlankCursor);
    Main_Widget w;

    w.showFullScreen();

    return a.exec();
}
