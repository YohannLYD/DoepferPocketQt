QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DofperPockerQt
TEMPLATE = app


SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp

include($$PWD/libs/QMidi/QMidi.pri)

HEADERS += \
    src/mainwindow.h
