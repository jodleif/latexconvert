#-------------------------------------------------
#
# Project created by QtCreator 2016-10-14T22:25:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = latex-generator
TEMPLATE = app
CONFIG += c++14

QMAKE_CXXFLAGS_DEBUG += -Weverything -Wno-c++98-compat

SOURCES += main.cpp\
        latexconvert.cpp

HEADERS  += latexconvert.h

FORMS    += latexconvert.ui
