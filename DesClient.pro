#-------------------------------------------------
#
# Project created by QtCreator 2012-02-29T21:32:01
#
#-------------------------------------------------

QT       += core gui sql

TARGET = DesClient
TEMPLATE = app

TRANSLATIONS = DesClient_zh_CN.ts

SOURCES += main.cpp\
        mainwindow.cpp \
    database.cpp \
    util/QReserveTableModel.cpp \
    util/MapDelegate.cpp \
    util/TimeEditDelegate.cpp \
    pages/09alarm.cpp \
    pages/10model.cpp \
    pages/01plan.cpp \
    pages/03status.cpp \
    pages/04des.cpp \
    pages/05slag.cpp \
    pages/06ele.cpp \
    pages/07hmp.cpp \
    pages/08info.cpp \
    pages/02standard.cpp \
    util/QQuery2TableModel.cpp \
    util/QQuery2TableModel.cpp \
    util/ToTheMiddleDelegate.cpp \
    util/ReadOnlyDelegate.cpp \
    pages/11pour.cpp

HEADERS  += mainwindow.h \
    database.h \
    util/QReserveTableModel.h \
    util/MapDelegate.h \
    util/TimeEditDelegate.h \
    pages/01plan.h \
    pages/03status.h \
    pages/04des.h \
    pages/05slag.h \
    pages/06ele.h \
    pages/07hmp.h \
    pages/08info.h \
    pages/09alarm.h \
    pages/10model.h \
    pages/02standard.h \
    util/QQuery2TableModel.h \
    util/QQuery2TableModel.h \
    util/ToTheMiddleDelegate.h \
    util/ReadOnlyDelegate.h \
    pages/11pour.h

FORMS    += mainwindow.ui \
    plandialog.ui \
    planConfirmDialog.ui \
    shiftDialog.ui

RESOURCES += \
    MyIcons.qrc


