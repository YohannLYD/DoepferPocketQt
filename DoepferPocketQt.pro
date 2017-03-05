QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DofperPocketQt
TEMPLATE = app


SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/settingswindow.cpp \
    src/eventwindow.cpp \
    src/paramwindow.cpp

include($$PWD/libs/QMidi/QMidi.pri)

HEADERS += \
    src/mainwindow.h \
    src/settingswindow.h \
    src/eventwindow.h \
    src/paramwindow.h

macx {
    QMAKE_CXXFLAGS += -std=c++11
    QMAKE_MAC_SDK = macosx10.12
#    _BOOST_PATH = /usr/local/Cellar/boost/1.62.0
#    INCLUDEPATH += "$${_BOOST_PATH}/include/"
#    LIBS += -L$${_BOOST_PATH}/lib
#    LIBS += $${_BOOST_PATH}/lib/libboost_system.a
}
