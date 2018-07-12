#ifndef CONVERTER_MAIN_TABLE_SHEDULE_H
#define CONVERTER_MAIN_TABLE_SHEDULE_H

#include <QObject>
#include <QFileInfo>

class Converter_main_table_shedule : public QObject
{
    Q_OBJECT
public:
    Converter_main_table_shedule(QString read_file, QObject *parent = 0);

private:
    QFile fileXmlOut, fileHtmlIn;

    void clearTag(QString *string, int *positionString, int indexStart, int indexEnd);
    void removeExcess(QString *string);
    void copyFilesInArchive();

};

#endif // CONVERTER_MAIN_TABLE_SHEDULE_H
