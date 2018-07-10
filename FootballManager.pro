#-------------------------------------------------
#
# Project created by QtCreator 2018-07-02T18:34:23
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FootballManager
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
        mainwindow.cpp \
    player.cpp \
    databaseutil.cpp \
    custommodel.cpp \
    playermodel.cpp \
    playerdialog.cpp \
    matchdialog.cpp \
    statisticdialog.cpp \
    matchmodel.cpp \
    aggregatesqlquerymodel.cpp \
    statisticmodel.cpp \
    matchesstatisticmodel.cpp \
    playersstatisticmodel.cpp \
    aboutdialog.cpp

HEADERS += \
        mainwindow.h \
    player.h \
    databaseutil.h \
    custommodel.h \
    playermodel.h \
    playerdialog.h \
    matchdialog.h \
    statisticdialog.h \
    match.h \
    matchmodel.h \
    aggregatesqlquerymodel.h \
    statisticmodel.h \
    statistic.h \
    matchesstatisticmodel.h \
    playersstatisticmodel.h \
    aboutdialog.h

FORMS += \
        mainwindow.ui \
    playerdialog.ui \
    matchdialog.ui \
    statisticdialog.ui \
    aboutdialog.ui
