#-------------------------------------------------
#
# Project created by QtCreator 2015-07-24T21:26:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CodeBackupTool
TEMPLATE = app

RC_ICONS = icon.ico

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    notifytray.cpp \
    backupcore.cpp

HEADERS  += mainwindow.h \
    notifytray.h \
    backupcore.h \
    backupthread.h

RESOURCES += \
    resource.qrc
