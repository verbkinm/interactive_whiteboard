/********************************************************************************
** Form generated from reading UI file 'leafer.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEAFER_H
#define UI_LEAFER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Leafer
{
public:
    QHBoxLayout *horizontalLayout;
    QPushButton *previos;
    QLabel *count;
    QPushButton *next;

    void setupUi(QWidget *Leafer)
    {
        if (Leafer->objectName().isEmpty())
            Leafer->setObjectName(QStringLiteral("Leafer"));
        Leafer->resize(492, 51);
        horizontalLayout = new QHBoxLayout(Leafer);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        previos = new QPushButton(Leafer);
        previos->setObjectName(QStringLiteral("previos"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/img/img/previos.png"), QSize(), QIcon::Normal, QIcon::Off);
        previos->setIcon(icon);
        previos->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(previos);

        count = new QLabel(Leafer);
        count->setObjectName(QStringLiteral("count"));
        count->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(count);

        next = new QPushButton(Leafer);
        next->setObjectName(QStringLiteral("next"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/img/img/next.png"), QSize(), QIcon::Normal, QIcon::Off);
        next->setIcon(icon1);
        next->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(next);


        retranslateUi(Leafer);

        QMetaObject::connectSlotsByName(Leafer);
    } // setupUi

    void retranslateUi(QWidget *Leafer)
    {
        Leafer->setWindowTitle(QApplication::translate("Leafer", "Form", Q_NULLPTR));
        previos->setText(QString());
        count->setText(QApplication::translate("Leafer", "x/n", Q_NULLPTR));
        next->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Leafer: public Ui_Leafer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEAFER_H
