#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "dcpconnection.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void netConnect();
    void disconnect();
    void join();
    void part();
    void conversation();
    void sendMessage();
    void showSpec();
    void about();

    void connected();
    void received(DCPMessage *);

private:
    void initActions();
    void initMenus();
    void initWidgets();

    QMenu *connectionMenu;
    QMenu *conversationMenu;
    QMenu *helpMenu;

    QAction *connectAct;
    QAction *disconnectAct;
    QAction *quitAct;
    QAction *joinAct;
    QAction *partAct;
    QAction *startConvoAct;
    QAction *sendMsgAct;
    QAction *specAct;
    QAction *aboutAct;

    DCPConnection *conn;

    QTextEdit *output;
    QLineEdit *input;
};

#endif // MAINWINDOW_H
