#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include "dcpconnection.h"
#include "dcpcommandprocessor.h"
#include "conversationwidget.h"

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
    void sockError(QAbstractSocket::SocketError);

    void groupJoined(QString, DCPMessage *);
    void groupLeft(QString, DCPMessage *);
    void groupReceived(QString, DCPMessage *);
    void privateReceived(QString, DCPMessage *);

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

    QTabWidget *tabs;
    QTextEdit *output;
    QLineEdit *input;

    DCPCommandProcessor *processor;

    QHash<QString, ConversationWidget *> widgetMapping;
};

#endif // MAINWINDOW_H
