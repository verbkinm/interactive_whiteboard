#ifndef CONVERTER_MAIN_TABLE_SHEDULE_H
#define CONVERTER_MAIN_TABLE_SHEDULE_H

#include <QFileInfo>
#include <QDomElement>

class Converter_main_table_shedule
{
public:
    Converter_main_table_shedule(QString read_file);

private:
    QFile fileXmlOut, fileHtmlIn;

    void        clearTag                (QString *string, int *positionString, int indexStart, int indexEnd);
    void        removeExcess            (QString *string);

// создание одного тега lessonTime
    QDomElement create_lessonTime       (QDomDocument &domDoc, QString number, QString timeBegin, QString timeEnd);

// создание всех тегов lessonTime
    void        create_lessonsTime      (QDomDocument &inputDoc, QDomDocument &outputDoc, QDomElement &parentTag);

//создание тега class
    void        create_class            (QDomDocument &inputDoc, QDomDocument &outputDoc, QDomElement &parentTag);

// создание тега lesson
    void create_lessons(QDomDocument &inputDoc, QDomDocument &outputDoc, QDomElement &parentTag, quint8 column);
};

#endif // CONVERTER_MAIN_TABLE_SHEDULE_H
