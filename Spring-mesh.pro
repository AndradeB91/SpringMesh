#-------------------------------------------------
#
# Project created by QtCreator 2015-09-05T16:33:09
#
#-------------------------------------------------

QT       += core gui opengl widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Spring-mesh
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    trackball.cpp \
    springnode.cpp \
    springsystem.cpp \
    edge.cpp \
    cell.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    trackball.h \
    springnode.h \
    springsystem.h \
    edge.h \
    cell.h

FORMS    += mainwindow.ui
