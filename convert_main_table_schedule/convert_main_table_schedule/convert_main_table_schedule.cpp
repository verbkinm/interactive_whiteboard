#include "converter_main_table_schedule.h"
#include <QDomDocument>
#include <QTextCodec>
#include <QDate>
#include <QDir>
#include <QDebug>

Converter_main_table_shedule::Converter_main_table_shedule(QString read_file, QObject *parent) : QObject(parent)
{
    qDebug() << "read_file" << read_file;

    fileHtmlIn.setFileName(read_file);
    if(!fileHtmlIn.exists()){
        printf("Converter_main_table_shedule error: file is not exists\n");
        return;
    }
    if(!fileHtmlIn.open(QIODevice::ReadOnly) ){
        printf("converter_main_table_shedule error: can not open the file %s\n", qPrintable(read_file));
        exit(3);
    }
    QByteArray html = fileHtmlIn.readAll();
    QTextCodec* defaultTextCodec = QTextCodec::codecForName("Windows-1251");
    QString unicode = defaultTextCodec->toUnicode(html);

    int indexStart  = unicode.indexOf("<table");

    QString buff;

    int j = 0;
    for (int i = unicode.size(); i != 0; --i, ++j)
        buff[j] = unicode[i];

    int indexEnd    = buff.indexOf(">elbaT/<");

    QString allFile = unicode.mid( indexStart, unicode.size() - indexStart - indexEnd + 1 ); // from <table to </Table>

    bool inTag          = false;
    bool attachedTag    = false;
    indexStart          = -1;
    indexEnd            = -1;

//Clear tags
    for (int i = 0; i < allFile.size(); ++i) {
        QChar c = allFile[i];
        if(inTag)
            allFile[i] = c.toLower();

        if(allFile[i] == '<'){
            if(inTag){
                attachedTag = true;
                continue;
            }
            inTag = true;
            indexStart = i;
        }
        if(allFile[i] == '>'){
            if(attachedTag){
                attachedTag = false;
                continue;
            }
            inTag = false;
            indexEnd = i;
            clearTag(&allFile , &i, indexStart, indexEnd);
        }
    }

    removeExcess(&allFile);
    QString xmlFormat = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xmlFormat.append(allFile);

    QDomDocument domDoc;
    domDoc.setContent(allFile);
    QDomElement root = domDoc.documentElement();
    QDomNodeList nodeList = root.childNodes();
    QDomNode firstCell, secondCell; //empty TD tag in first TR, and TD with rowspan(day of week)
    firstCell   = nodeList.at(0).firstChild();
    secondCell  = nodeList.at(1).firstChild();
    nodeList.at(0).removeChild(firstCell);
    nodeList.at(1).removeChild(secondCell);

//OUTPUT FILE
    fileXmlOut.setFileName(QFileInfo(read_file).path() + "/tmp");//(write_file);
    fileXmlOut.open(QIODevice::ReadWrite | QIODevice::Text);
    fileXmlOut.write(domDoc.toByteArray());
//COPY FILES IN ARCHIVE
    copyFilesInArchive();
//CLOSE FILES
    fileXmlOut.close();
    fileHtmlIn.close();

//    fileXmlOut.remove();
//    fileHtmlIn.remove();

    printf("converter_main_table_shedule creating OK!\n");
}
void Converter_main_table_shedule::clearTag(QString *string, int *positionString, int indexStart, int indexEnd)
{
    int lenghtString = indexEnd - indexStart;
    QString buff = string->mid(indexStart,lenghtString + 1);
    if(buff.indexOf(" ") > 0){
        int indexSpace = buff.indexOf(" ");
        string->remove(indexSpace + indexStart, lenghtString - indexSpace);
        *positionString = indexStart + indexSpace;
    }
}
void Converter_main_table_shedule::removeExcess(QString *string)
{
    string->remove("&nbsp;");
    string->remove("<caption>");string->remove("</caption>");
    string->remove("<h1>");string->remove("</h1>");
    string->remove("<strong>");string->remove("</strong>");
    string->remove("Расписание классов");
}
void Converter_main_table_shedule::copyFilesInArchive()
{
//    QDate date = (tomorrow ? MySpace::checkDate(QDate::currentDate(), '+') : QDate::currentDate());
//    if(date.dayOfWeek() == 7)
//        date = MySpace::checkDate(date, '+');

//    QString directory_of_date = date.toString("yyyy") + \
//            QString(PATH_SPLITER) + \
//            date.toString("MM");

//    QDir dir;
//    dir.mkpath(SHARE_ARCHIVE_PATH + QString(PATH_SPLITER) + directory_of_date);
//    dir.mkpath(LOCAL_ARCHIVE_PATH + QString(PATH_SPLITER) + directory_of_date);

//    QFile tmpFileShare(SHARE_ARCHIVE_PATH+\
//                       QString(PATH_SPLITER) + directory_of_date + \
//                       QString(PATH_SPLITER) + date.toString("dd")+".html");

//    tmpFileShare.open(QIODevice::WriteOnly | QIODevice::Text);
//    fileHtmlIn.seek(0);
//    tmpFileShare.write(fileHtmlIn.readAll());
//    tmpFileShare.close();

//    QFile tmpFileLocal(LOCAL_ARCHIVE_PATH+\
//                       QString(PATH_SPLITER) + directory_of_date + \
//                       QString(PATH_SPLITER) + date.toString("dd") + ".xml");
//    fileXmlOut.seek(0);
//    tmpFileLocal.open(QIODevice::WriteOnly | QIODevice::Text);
//    tmpFileLocal.write(fileXmlOut.readAll());
//    tmpFileLocal.close();
}
