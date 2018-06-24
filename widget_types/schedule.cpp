#include "schedule.h"
#include <QDomComment>
#include <QFile>

#include <QDebug>

void traverseNode(const QDomNode& node)
{
   QDomNode domNode = node.firstChild();
   while(!domNode.isNull()) {
       if(domNode.isElement()) {
          QDomElement domElement = domNode.toElement();
          if(!domElement.isNull()) {
              if(domElement.tagName() == "contact") {
                  qDebug() << domElement.attribute("number", "");
                  qDebug() << "Attr: "
                           << domElement.attribute("number", "");
              }
              else {
                  qDebug() << "TagName: " << domElement.tagName() << "Attr: " << domElement.attribute("name", "") \
                           << "\tText: " << domElement.text();
             }
          }
       }
       traverseNode(domNode);
       domNode = domNode.nextSibling();
    }
}

Schedule::Schedule() : QTableWidget()
{
    QDomDocument domDoc;
    QFile        file("/home/user/myProg/QT/interactive_whiteboard/schedule.xml");

    if(file.open(QIODevice::ReadOnly)) {
        if(domDoc.setContent(&file)) {
            QDomElement domElement= domDoc.documentElement();
            traverseNode(domElement);
        }
        file.close();
    }

}
