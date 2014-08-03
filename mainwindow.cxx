#include "mainwindow.h"

#include <KApplication>
#include <KAboutApplicationDialog>
#include <KCmdLineArgs>
#include <QtGui>
#include <QDesktopServices> // open URL
#include <QUrl> // actual URL to open

#include "connectdialog.h"
#include "utility.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initActions();
    initMenus();
    initWidgets();

    QString welcome = tr("Welcome to Gilligan - connect to a network to start "
                         "chatting.");
    statusBar()->showMessage(welcome);

    setWindowTitle(tr("Not Connected - Gilligan"));
    setMinimumSize(480, 320);
    resize(700, 500);

    conn = NULL;
    processor = NULL;
    wizard = NULL;
}

MainWindow::~MainWindow()
{
    delete processor;
    delete conn;
}



void MainWindow::maybeCreateConn()
{
    if(conn == NULL)
    {
        conn = new DCPConnection;

        connect(conn, SIGNAL(networkConnected()),
                this, SLOT(connected()));
        connect(conn, SIGNAL(messageReceived(DCPMessage*)),
                this, SLOT(received(DCPMessage*)));
        connect(conn, SIGNAL(errorReceived(QAbstractSocket::SocketError)),
                this, SLOT(sockError(QAbstractSocket::SocketError)));

        if(processor != NULL) delete processor;
        processor = new DCPCommandProcessor(conn, this);

        connect(conn, SIGNAL(messageReceived(DCPMessage*)),
                processor, SLOT(rawMessageReceived(DCPMessage*)));

        connect(processor, SIGNAL(pingReceived(DCPMessage*)),
                processor, SLOT(defaultPingReceived(DCPMessage*)));

        connect(processor, SIGNAL(groupJoined(QString,DCPMessage*)),
                this, SLOT(groupJoined(QString,DCPMessage*)));
        connect(processor, SIGNAL(groupLeft(QString,DCPMessage*)),
                this, SLOT(groupLeft(QString,DCPMessage*)));

        connect(processor, SIGNAL(groupMessageReceived(QString,DCPMessage*)),
                this, SLOT(groupReceived(QString,DCPMessage*)));
        connect(processor, SIGNAL(privateMessageReceived(QString,DCPMessage*)),
                this, SLOT(privateReceived(QString,DCPMessage*)));
    }
}




/**** Actions ****/

void MainWindow::startRegistering()
{
    if(wizard != NULL) return;

    wizard = new RegistrationWizard(this);
    connect(wizard, SIGNAL(finished(int)), this, SLOT(finishRegistering(int)));
    wizard->show();
}

void MainWindow::finishRegistering(int result)
{
    if(result == QDialog::Accepted)
    {
        QString server = wizard->field("serverName").toString();

        if(server.length() == 0)
            server = "elizabethmyers.me.uk";

        maybeCreateConn();
        conn->registerAndConnect(server, wizard->field("handle").toString(),
                                 wizard->field("passphrase").toString(),
                                 wizard->clientName(),
                                 wizard->field("gecos").toString());
    }

    wizard->deleteLater();
    wizard = NULL;
}

/** netConnect - connect to a network */
void MainWindow::netConnect()
{
    ConnectDialog dialog;
    if(dialog.exec() == QDialog::Accepted)
    {
        maybeCreateConn();
        conn->connectTo(dialog.server(), dialog.handle(), dialog.passphrase(),
                        dialog.client());
    }
}



void MainWindow::sockError(QAbstractSocket::SocketError error)
{
    QString friendlyError = tr("Unknown error ") + QString::number(error);
    switch(error)
    {
    case QAbstractSocket::ConnectionRefusedError:
        friendlyError = tr("Connection refused");
        break;
    case QAbstractSocket::RemoteHostClosedError:
        friendlyError = tr("Remote host closed the connection");
        break;
    case QAbstractSocket::HostNotFoundError:
        friendlyError = tr("Host not found");
        break;
    case QAbstractSocket::SslHandshakeFailedError:
        friendlyError = tr("SSL handshake failed");
        break;
    case QAbstractSocket::SocketResourceError:
        friendlyError = tr("Out of sockets");
        break;
    case QAbstractSocket::SocketTimeoutError:
        friendlyError = tr("Connection timed out");
        break;
    case QAbstractSocket::NetworkError:
        friendlyError = tr("Network error");
        break;
    case QAbstractSocket::AddressInUseError:
        friendlyError = tr("Address already in use");
        break;
    case QAbstractSocket::SocketAddressNotAvailableError:
        friendlyError = tr("Socket address not available");
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        friendlyError = tr("Operation not supported");
        break;
    case QAbstractSocket::UnfinishedSocketOperationError:
        friendlyError = tr("Unfinished socket operation");
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        friendlyError = tr("Proxy requires authentication");
        break;
    case QAbstractSocket::ProxyConnectionRefusedError:
        friendlyError = tr("Proxy: connection refused");
        break;
    case QAbstractSocket::ProxyConnectionClosedError:
        friendlyError = tr("Proxy: connection closed");
        break;
    case QAbstractSocket::ProxyConnectionTimeoutError:
        friendlyError = tr("Proxy: connection timed out");
        break;
    case QAbstractSocket::ProxyNotFoundError:
        friendlyError = tr("Proxy: not found");
        break;
    case QAbstractSocket::ProxyProtocolError:
        friendlyError = tr("Proxy: protocol error");
        break;
    default:
        friendlyError = tr("Unknown socket error");
    }

    QString message = tr("Connection problem: %1.").arg(friendlyError);
    QMessageBox::critical(this, tr("Socket error"), message);

    emit disconnect();
}



void MainWindow::connected()
{
    output->insertHtml("Now connected<br>\n");
    setWindowTitle(tr("Connected - Gilligan"));
    connectAct->setEnabled(false);
    disconnectAct->setEnabled(true);
    joinAct->setEnabled(true);
    startConvoAct->setEnabled(true);
}



void MainWindow::received(DCPMessage *message)
{
    output->insertHtml(messageRepr(message));
}



void MainWindow::groupJoined(QString name, DCPMessage *message)
{
    if(!widgetMapping.keys().contains(name))
    {
        ConversationWidget *w = new ConversationWidget(name, true);
        tabs->addTab(w, name);
        widgetMapping.insert(name, w);
    }

    if(message->source != conn->handle())
    {
        widgetMapping.value(name)->messageReceived(name, message);
    }
}



void MainWindow::groupReceived(QString name, DCPMessage *message)
{
    if(!widgetMapping.keys().contains(name))
    {
        fprintf(stderr, "received a message from a group we aren't joined to?\n");
        this->groupJoined(name, message);
    }

    widgetMapping.value(name)->messageReceived(name, message);
}



void MainWindow::groupLeft(QString, DCPMessage *)
{
    // TODO STUB
}



void MainWindow::privateReceived(QString name, DCPMessage *message)
{
    if(!widgetMapping.keys().contains(name))
    {
        ConversationWidget *w = new ConversationWidget(name, false);
        tabs->addTab(w, name);
        widgetMapping.insert(name, w);
        w->messageReceived(name, message);
        return;
    }

    widgetMapping.value(name)->messageReceived(name, message);
}



void MainWindow::disconnect()
{
    conn->disconnectFrom();

    setWindowTitle(tr("Not Connected - Gilligan"));
    output->insertHtml(tr("Disconnected.<br>\n"));

    disconnectAct->setEnabled(false);
    connectAct->setEnabled(true);

    startConvoAct->setEnabled(false);
    joinAct->setEnabled(false);
}



void MainWindow::join()
{
    if(processor == NULL) return;

    QString group = QInputDialog::getText(this, tr("Group Name"),
                                          tr("Enter the name of the group to join."));
    DCPMessage *join = new DCPMessage("*",
                                      group,
                                      "group-enter",
                                      QMultiHash<QString,QString>());
    processor->sendMessage(join);
    delete join;
}



void MainWindow::part()
{
    // TODO STUB
}



void MainWindow::conversation()
{
    QString person = QInputDialog::getText(this, tr("Start Chat"),
                                           tr("Enter the handle of the person you want to chat."));
    if(!widgetMapping.keys().contains(person))
    {
        ConversationWidget *w = new ConversationWidget(person, false);
        tabs->addTab(w, person);
        widgetMapping.insert(person, w);
        tabs->setCurrentWidget(w);
        return;
    }
    else
    {
        tabs->setCurrentWidget(widgetMapping.value(person));
    }
}



void MainWindow::sendMessage()
{
    QString dest, command;
    QMultiHash<QString, QString> params;

    QString raw_input = input->text();

    if(processor == NULL) return;
    // clear out the text
    input->setText("");

    if(raw_input[0] == '/')
    {
        // Do a command.
        int cmd_length = raw_input.indexOf(' ');
        command = raw_input.mid(1, cmd_length - 1);

        raw_input = raw_input.mid(cmd_length + 1);

        bool inbracket = false;
        bool esc = false;
        bool found_dest = false;
        QString key = "";
        QString value = "";
        bool do_key = true;
        for(int ch = 0; ch < raw_input.length(); ch++)
        {
            if(inbracket)
            {
                if(raw_input[ch] == ']' && !esc)
                {
                    inbracket = false;
                    params.insert(key, value);
                    key = "";  value = "";
                }
                else if(raw_input[ch] == '=' && !esc)
                {
                    do_key = false;
                }
                else
                {
                    if(esc || raw_input[ch] != '\\')
                    {
                        if(do_key) key += raw_input[ch];
                        else value += raw_input[ch];
                    }
                }
            }
            else
            {
                if(raw_input[ch] == '[' && !esc)
                {
                    inbracket = true;
                    do_key = true;
                }
                else
                {
                    if(!found_dest && (esc || raw_input[ch] != '\\'))
                    {
                        if(raw_input[ch] != ' ') dest += raw_input[ch];
                        else found_dest = true;
                    }
                }
            }

            esc = (ch == '\\' && !esc);
        }
    } else {
        ConversationWidget *conv = dynamic_cast<ConversationWidget *>(tabs->currentWidget());
        if(conv == NULL)
        {
            output->insertHtml(tr("You can't insert text into the current buffer.<br>\n"));
            return;
        }

        dest = widgetMapping.key(conv);
        command = "message";
        params.insert("body", raw_input);
    }

    DCPMessage *msg = new DCPMessage("*", dest, command, params);
    output->insertHtml(messageRepr(msg));
    processor->sendMessage(msg);
    delete msg;
}



/** showSpec - show the current draft spec */
void MainWindow::showSpec()
{
    QDesktopServices::openUrl(QUrl("http://foxkit.us/spec.pdf"));
}



/** about - show information about us */
void MainWindow::about()
{
    KAboutApplicationDialog ad(KCmdLineArgs::aboutData());
    ad.exec();
}



/** initActions - initialise the actions */
void MainWindow::initActions()
{
    registerAct = new QAction(tr("&Register..."), this);
    registerAct->setStatusTip(tr("Register with DCP."));
    connect(registerAct, SIGNAL(triggered()), this, SLOT(startRegistering()));

    connectAct = new QAction(tr("&Connect..."), this);
    connectAct->setStatusTip(tr("Connect to a network."));
    connect(connectAct, SIGNAL(triggered()), this, SLOT(netConnect()));

    disconnectAct = new QAction(tr("&Disconnect"), this);
    disconnectAct->setStatusTip(tr("Disconnect from the network."));
    disconnectAct->setEnabled(false);
    connect(disconnectAct, SIGNAL(triggered()), this, SLOT(disconnect()));

    quitAct = new QAction(tr("&Quit Gilligan"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Close Gilligan."));
    connect(quitAct, SIGNAL(triggered()), kapp, SLOT(quit()));

    joinAct = new QAction(tr("&Join Group..."), this);
    joinAct->setStatusTip(tr("Join a group."));
    joinAct->setEnabled(false);
    connect(joinAct, SIGNAL(triggered()), this, SLOT(join()));

    partAct = new QAction(tr("&Leave"), this);
    partAct->setShortcuts(QKeySequence::Close);
    partAct->setStatusTip(tr("Leave the current conversation."));
    partAct->setEnabled(false);
    connect(partAct, SIGNAL(triggered()), this, SLOT(part()));

    startConvoAct = new QAction(tr("Start &Chat..."), this);
    startConvoAct->setShortcuts(QKeySequence::New);
    startConvoAct->setStatusTip(tr("Start a new private conversation."));
    startConvoAct->setEnabled(false);
    connect(startConvoAct, SIGNAL(triggered()), this, SLOT(conversation()));

    specAct = new QAction(tr("View DCP &Spec"), this);
    specAct->setStatusTip(tr("Open the current draft DCP spec using your Web browser."));
    connect(specAct, SIGNAL(triggered()), this, SLOT(showSpec()));

    aboutAct = new QAction(tr("&About Gilligan..."), this);
    aboutAct->setStatusTip(tr("View information about the client."));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}



/** initMenus - initialise the menus */
void MainWindow::initMenus()
{
    connectionMenu = menuBar()->addMenu(tr("&Connection"));
    connectionMenu->addAction(registerAct);
    connectionMenu->addSeparator();
    connectionMenu->addAction(connectAct);
    connectionMenu->addAction(disconnectAct);
    connectionMenu->addSeparator();
    connectionMenu->addAction(quitAct);

    conversationMenu = menuBar()->addMenu(tr("Con&versation"));
    conversationMenu->addAction(joinAct);
    conversationMenu->addAction(partAct);
    conversationMenu->addSeparator();
    conversationMenu->addAction(startConvoAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(specAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAct);
}



/** initWidgets - initialise the widgets */
void MainWindow::initWidgets()
{
    QWidget *central = new QWidget;
    setCentralWidget(central);

    output = new QTextEdit;
    output->setReadOnly(true);
    output->document()->setDefaultStyleSheet("span.message { display: block; font-family: monospace; }"
                                             //"span.source { font-weight: bold; }"
                                             "span.destination { font-style: italic; }"
                                             "span.command { font-weight: bold; }"
                                             "span.param { background-color: #eee; }"
                                             "span.key { font-weight: bold; }");

    input = new QLineEdit;
    connect(input, SIGNAL(returnPressed()), this, SLOT(sendMessage()));

    tabs = new QTabWidget;
    tabs->addTab(output, tr("Server Console"));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(tabs);
    layout->addWidget(input);
    central->setLayout(layout);
}
