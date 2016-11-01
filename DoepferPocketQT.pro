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

macx {
    QMAKE_CXXFLAGS += -std=c++11

    _BOOST_PATH = /usr/local/Cellar/boost/1.62.0
    INCLUDEPATH += "$${_BOOST_PATH}/include/"
    LIBS += -L$${_BOOST_PATH}/lib
    LIBS += $${_BOOST_PATH}/lib/libboost_system.a
}
