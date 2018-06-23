#include "main_widget/main_widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Main_Widget w;

    w.showFullScreen();
//    w.show();

    return a.exec();
}
