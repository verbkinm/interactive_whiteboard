/********************************************************************************
** Form generated from reading UI file 'viewer.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWER_H
#define UI_VIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_viewer
{
public:
    QVBoxLayout *verticalLayout_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_4;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *prevois;
    QLabel *pages;
    QPushButton *next;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *real_size;
    QPushButton *plus;
    QPushButton *minus;

    void setupUi(QWidget *viewer)
    {
        if (viewer->objectName().isEmpty())
            viewer->setObjectName(QStringLiteral("viewer"));
        viewer->resize(727, 530);
        verticalLayout_2 = new QVBoxLayout(viewer);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        scrollArea = new QScrollArea(viewer);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 707, 478));
        verticalLayout_4 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_2->addWidget(scrollArea);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, -1, -1);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        prevois = new QPushButton(viewer);
        prevois->setObjectName(QStringLiteral("prevois"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/img/img/previos.png"), QSize(), QIcon::Normal, QIcon::Off);
        prevois->setIcon(icon);

        horizontalLayout->addWidget(prevois);

        pages = new QLabel(viewer);
        pages->setObjectName(QStringLiteral("pages"));

        horizontalLayout->addWidget(pages);

        next = new QPushButton(viewer);
        next->setObjectName(QStringLiteral("next"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/img/img/next.png"), QSize(), QIcon::Normal, QIcon::Off);
        next->setIcon(icon1);

        horizontalLayout->addWidget(next);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        real_size = new QPushButton(viewer);
        real_size->setObjectName(QStringLiteral("real_size"));
        real_size->setAutoDefault(false);
        real_size->setFlat(false);

        horizontalLayout->addWidget(real_size);

        plus = new QPushButton(viewer);
        plus->setObjectName(QStringLiteral("plus"));

        horizontalLayout->addWidget(plus);

        minus = new QPushButton(viewer);
        minus->setObjectName(QStringLiteral("minus"));

        horizontalLayout->addWidget(minus);


        verticalLayout_2->addLayout(horizontalLayout);


        retranslateUi(viewer);

        real_size->setDefault(false);


        QMetaObject::connectSlotsByName(viewer);
    } // setupUi

    void retranslateUi(QWidget *viewer)
    {
        viewer->setWindowTitle(QApplication::translate("viewer", "viewer", Q_NULLPTR));
        label->setText(QApplication::translate("viewer", "TextLabel", Q_NULLPTR));
        prevois->setText(QString());
        pages->setText(QApplication::translate("viewer", "0/0", Q_NULLPTR));
        next->setText(QString());
        real_size->setText(QApplication::translate("viewer", "100 %", Q_NULLPTR));
        plus->setText(QApplication::translate("viewer", "+", Q_NULLPTR));
        minus->setText(QApplication::translate("viewer", "-", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class viewer: public Ui_viewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWER_H
