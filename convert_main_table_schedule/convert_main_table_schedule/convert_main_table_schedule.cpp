#include "converter_main_table_schedule.h"
#include <QDomDocument>
#include <QTextCodec>
#include <QDebug>

Converter_main_table_shedule::Converter_main_table_shedule(QString read_file)
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
    domDoc.setContent(xmlFormat);
    QDomElement root = domDoc.documentElement();
    QDomNodeList nodeList = root.childNodes();
    QDomNode rmCell;
    rmCell   = nodeList.at(0).firstChild(); //пустая ячейка
    nodeList.at(0).removeChild(rmCell);
    rmCell   = nodeList.at(0).firstChild(); //#
    nodeList.at(0).removeChild(rmCell);
    rmCell   = nodeList.at(0).firstChild(); //Время
    nodeList.at(0).removeChild(rmCell);
    rmCell   = nodeList.at(1).firstChild(); //день недели
    QString title = rmCell.toElement().text();
    nodeList.at(1).removeChild(rmCell);

    QDomDocument domDocOutput;
    QDomElement rootOutPut = domDocOutput.createElement("schedule");
    domDocOutput.appendChild(rootOutPut);

    QDomElement domElementTitle = domDocOutput.createElement("title");
    QDomText domTextTitle = domDocOutput.createTextNode(title);
    domElementTitle.appendChild(domTextTitle);
    rootOutPut.appendChild(domElementTitle);

    create_lessonsTime(domDoc, domDocOutput, rootOutPut);
    create_class(domDoc, domDocOutput, rootOutPut);

//OUTPUT FILE
    fileXmlOut.setFileName(QFileInfo(read_file).path() + "/output.xml");//(write_file);
    fileXmlOut.open(QIODevice::ReadWrite | QIODevice::Text);
    fileXmlOut.write(domDocOutput.toByteArray());


    printf("Creating file - %s - OK!\n", qPrintable(QFileInfo(fileXmlOut).filePath()) );

//CLOSE FILES
    fileXmlOut.close();
    fileHtmlIn.close();
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
QDomElement Converter_main_table_shedule::create_lessonTime(QDomDocument &domDoc, QString number, \
                                                            QString timeBegin, QString timeEnd)
{
    QDomElement domElement          = domDoc.createElement("lessonTime");

    QDomElement domElementNumber    = domDoc.createElement("number");
    QDomText    domText             = domDoc.createTextNode(number);
    domElementNumber.appendChild(domText);

    QDomElement domElementTimeBegin = domDoc.createElement("timeBegin");
    domText                         = domDoc.createTextNode(timeBegin);
    domElementTimeBegin.appendChild(domText);

    QDomElement domElementTimeEnd   = domDoc.createElement("timeEnd");
    domText                         = domDoc.createTextNode(timeEnd);

    domElementTimeEnd.appendChild(domText);

    domElement.appendChild(domElementNumber);
    domElement.appendChild(domElementTimeBegin);
    domElement.appendChild(domElementTimeEnd);

    return domElement;
}
void Converter_main_table_shedule::create_lessonsTime(QDomDocument &inputDoc, QDomDocument &outputDoc, \
                                                      QDomElement &parentTag)
{
    QDomNodeList nodeList = inputDoc.documentElement().childNodes();

    for (int i = 1; i < nodeList.length(); ++i) {
        QDomNodeList nodeSubList = nodeList.at(i).toElement().childNodes();
        QString number      = nodeSubList.at(0).toElement().text();
        QString time        = nodeSubList.at(1).toElement().text();
        QString timeBegin   = QString(time.split("-").at(0)).replace(" ", "");
        QString timeEnd     = QString(time.split("-").at(1)).replace(" ", "");

//        QDomComment domComment =  outputDoc.createComment("Урок №" + number);
//        parentTag.appendChild(domComment);
        parentTag.appendChild(create_lessonTime(outputDoc, number, timeBegin, timeEnd) );
    }
}
void Converter_main_table_shedule::create_class(QDomDocument &inputDoc, QDomDocument &outputDoc, \
                                                QDomElement &parentTag)
{
    QDomNodeList nodeList = inputDoc.documentElement().firstChild().childNodes();

    for (int column = 0; column < nodeList.length(); ++column) {
        QDomElement domElement  = outputDoc.createElement("class");
        QDomAttr    domAttr     = outputDoc.createAttribute("name");
        domAttr.setValue(nodeList.at(column).toElement().text());
        domElement.setAttributeNode(domAttr);

        create_lessons(inputDoc, outputDoc, domElement, column+2);

        parentTag.appendChild(domElement);
    }
}
void Converter_main_table_shedule::create_lessons(QDomDocument &inputDoc, QDomDocument &outputDoc, \
                                                         QDomElement &parentTag, quint8 column)
{
    QDomNodeList nodeList = inputDoc.documentElement().childNodes();

// строка таблицы
    for (int row = 1; row < nodeList.length(); ++row) {
// +2 - сдвиг от номера урока и времени
        QDomNodeList nodeSubList = nodeList.at(row).childNodes();
        QDomElement  domElementLesson = outputDoc.createElement("lesson");

        QDomNode tableNode = nodeSubList.at(column).firstChild();
        if(tableNode.toElement().tagName() == "table"){

            QString lessonName = tableNode.firstChild().firstChild().toElement().text();


//первый тег TD первой строки, в котором название урока  и далее фамилии учителей
            QDomNode firstTrTDs = tableNode.firstChild().firstChild().nextSibling();
// вторая строка, где записаны номера кабинетов
            QDomNode secondTrTDs = tableNode.firstChild().nextSibling().firstChild();

            int counter = 1;
            while(!firstTrTDs.isNull())
            {
                if(counter == 2){
                    QDomElement separatorElement = outputDoc.createElement("separator");
                    domElementLesson.appendChild(separatorElement);
                    counter = 1;
                    firstTrTDs = firstTrTDs.nextSibling();
                    continue;
                }
//<name>
                QDomElement domElementName    = outputDoc.createElement("name");
                QDomText    domText           = outputDoc.createTextNode(lessonName);
                domElementName.appendChild(domText);
//<teacher>
                QDomElement domElementTeacher = outputDoc.createElement("teacher");
                domText = outputDoc.createTextNode(firstTrTDs.toElement().text());
                domElementTeacher.appendChild(domText);

//<place>
                QDomElement domElementPlace   = outputDoc.createElement("place");
                domText     = outputDoc.createTextNode(secondTrTDs.toElement().text());
                domElementPlace.appendChild(domText);

                domElementLesson.appendChild(domElementName);
                domElementLesson.appendChild(domElementPlace);
                domElementLesson.appendChild(domElementTeacher);

                firstTrTDs  = firstTrTDs.nextSibling();
                secondTrTDs = secondTrTDs.nextSibling();

                counter++;
            }
        }

//        QDomComment domComment =  outputDoc.createComment("Урок №" + QString::number(row) );
//        parentTag.appendChild(domComment);

        parentTag.appendChild(domElementLesson);
    }

}
