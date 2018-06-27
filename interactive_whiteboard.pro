#-------------------------------------------------
#
# Project created by QtCreator 2018-06-21T16:23:04
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = interactive_whiteboard
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    main_widget/main_widget.cpp \
    mini_widget/mini_widget.cpp \
    content/content.cpp \
    widget_types/clock.cpp \
    convert_main_table_schedule/convert_main_table_schedule.cpp \
    widget_types/schedule.cpp \
    widget_types/cell/cell.cpp \
    widget_types/myWidgets/mytablewidget.cpp \
    widget_types/myWidgets/myTabelWidgetEventFilter/mytabelwidgeteventfilter.cpp


HEADERS += \
    main_widget/main_widget.h \
    mini_widget/mini_widget.h \
    content/content.h \
    widget_types/clock.h \
    convert_main_table_schedule/converter_main_table_schedule.h \
    widget_types/schedule.h \
    widget_types/cell/cell.h \
    widget_types/mytablewidget.h \
    widget_types/myWidgets/myTabelWidgetEventFilter/mytabelwidgeteventfilter.h
    widget_types/myWidgets/mytablewidget.h

RESOURCES += \
    img.qrc
