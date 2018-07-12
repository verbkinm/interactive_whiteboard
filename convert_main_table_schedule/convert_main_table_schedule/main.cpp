#include <QCoreApplication>
#include "converter_main_table_schedule.h"

#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString read_file = argv[1];

//    std::cout << qPrintable(read_file) << std::endl;

    Converter_main_table_shedule app(read_file);

//    return a.exec();
}
