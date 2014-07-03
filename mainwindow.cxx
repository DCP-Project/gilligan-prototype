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
}

MainWindow::~MainWindow()
{
    delete conn;
}




/**** Actions ****/

/** netConnect - connect to a network */
void MainWindow::netConnect()
{
    ConnectDialog dialog;
    if(dialog.exec() == QDialog::Accepted)
    {
        conn = new DCPConnection;

        connect(conn, SIGNAL(networkConnected()),
                this, SLOT(connected()));
        connect(conn, SIGNAL(messageReceived(DCPMessage*)),
                this, SLOT(received(DCPMessage*)));
        connect(conn, SIGNAL(errorReceived(QAbstractSocket::SocketError)),
                this, SLOT(sockError(QAbstractSocket::SocketError)));

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
    }

    QString message = tr("Connection problem: %1.").arg(friendlyError);
    QMessageBox::critical(this, tr("Socket error"), message);
}



void MainWindow::connected()
{
    output->insertHtml("Now connected<br>\n");
    setWindowTitle(tr("Connected - Gilligan"));
    connectAct->setEnabled(false);
    disconnectAct->setEnabled(true);
}



void MainWindow::received(DCPMessage *message)
{
    output->insertHtml(messageRepr(message));
}



void MainWindow::disconnect()
{
    delete conn;
    conn = NULL;

    setWindowTitle(tr("Not Connected - Gilligan"));
    output->insertHtml("Disconnected.");

    disconnectAct->setEnabled(false);
    connectAct->setEnabled(true);
}



void MainWindow::join()
{
    // TODO STUB
}



void MainWindow::part()
{
    // TODO STUB
}



void MainWindow::conversation()
{
    // TODO STUB
}



void MainWindow::sendMessage()
{
    // TODO STUB
    /*
def interpret(text):
   kval = defaultdict(list)
   dest = ''

   # State for the thing
   inbracket = False
   esc = False
   found_dest = False
   key = ''
   val = ''
   do_key = True
   for ch in text:
         if inbracket:
            if ch == ']' and not esc:
               # Found a key:value pair
               inbracket = False
               kval[key].append(val)
               key = val = ''
            elif ch == '=' and not esc:
               do_key = False
            else:
               if esc or ch != '\\':
                  if do_key:
                     key += ch
                  else:
                     val += ch
         else:
            if ch == '[' and not esc:
               inbracket = True
               do_key = True
            else:
               if not found_dest and (esc or ch != '\\'):
                  if ch != ' ':
                     dest += ch
                  else:
                     found_dest = True

         esc = (ch == '\\' and not esc)

   return (dest, kval)
*/
    QString dest, command;
    QMultiHash<QString, QString> params;

    QString raw_input = input->text();
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
        /* XXX */
        // dest = currentConversation->name();
        // command = "message";
        // params.insert("body", raw_input);
        return;
    }

    DCPMessage *msg = new DCPMessage("*", dest, command, params);
    output->insertHtml(messageRepr(msg));
    conn->sendMessage(msg);
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

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(output);
    layout->addWidget(input);
    central->setLayout(layout);
}
