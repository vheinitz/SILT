

QT       += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SILTImport
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    persistence.cpp

HEADERS  += mainwindow.h \
    persistence.h

FORMS    += mainwindow.ui
