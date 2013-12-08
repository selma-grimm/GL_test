#-------------------------------------------------
#
# Project created by QtCreator 2013-12-06T17:19:36
# by Marina Podtinnaya
#
#-------------------------------------------------

QT       += core gui concurrent

#DEFINES += RUN_TEST
contains(DEFINES, RUN_TEST) {
QT += testlib
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GL_test
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp
contains(DEFINES, RUN_TEST) {
SOURCES += \
    CRCTestClass.cpp
}

HEADERS  += MainWindow.h
contains(DEFINES, RUN_TEST) {
HEADERS += \
    CRCTestClass.h
}

FORMS    += MainWindow.ui

QMAKE_CXXFLAGS += -std=c++11
