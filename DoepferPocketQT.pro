QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DofperPocketQT
TEMPLATE = app


SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/settingswindow.cpp

include($$PWD/libs/QMidi/QMidi.pri)

HEADERS += \
    src/mainwindow.h \
    src/settingswindow.h
