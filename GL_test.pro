#-------------------------------------------------
#
# Project created by QtCreator 2013-12-06T17:19:36
#
#-------------------------------------------------

QT       += core gui concurrent testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GL_test
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    CRCTestClass.cpp

HEADERS  += MainWindow.h \
    CRCTestClass.h

FORMS    += MainWindow.ui

QMAKE_CXXFLAGS += -std=c++11
