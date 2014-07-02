#-------------------------------------------------
#
# Project created by QtCreator 2014-07-01T19:15:14
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gilligan
TEMPLATE = app


SOURCES += main.cxx\
        mainwindow.cxx \
    dcpconnection.cxx \
    dcpmessage.cxx \
    utility.cxx

HEADERS  += mainwindow.h \
    dcpconnection.h \
    dcpmessage.h \
    utility.h
