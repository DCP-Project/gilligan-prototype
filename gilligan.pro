#-------------------------------------------------
#
# Project created by QtCreator 2014-07-01T19:15:14
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gilligan
TEMPLATE = app

kde {
    INCLUDEPATH += /usr/include/KDE
    LIBS += -lkdecore -lkdeui
    DEFINES += -DHAVE_KDE
}

SOURCES += main.cxx\
        mainwindow.cxx \
    dcpconnection.cxx \
    dcpmessage.cxx \
    utility.cxx \
    connectdialog.cxx \
    dcpcommandprocessor.cxx \
    conversationwidget.cxx \
    registrationwizard.cxx

HEADERS  += mainwindow.h \
    dcpconnection.h \
    dcpmessage.h \
    utility.h \
    connectdialog.h \
    dcpcommandprocessor.h \
    conversationwidget.h \
    registrationwizard.h \
    registrationwizard_private.h
