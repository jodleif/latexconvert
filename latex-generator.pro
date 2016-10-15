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

QMAKE_CXXFLAGS_DEBUG += -Weverything -Wno-c++98-compat -march=native -mtune=native

SOURCES += main.cpp\
        latexconvert.cpp \
    latex/latexcreator.cpp \
    model/configoptions.cpp \
    latex/formatfromclipboard.cpp

HEADERS  += \
    latex/latexcreator.h \
    latexconvert.h \
    model/configoptions.h \
    latex/formatfromclipboard.h

FORMS    += latexconvert.ui
