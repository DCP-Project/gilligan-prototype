#include "mainwindow.h"

#include <QDesktopServices> // open URL
#include <QUrl> // actual URL to open

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initActions();
    initMenus();
    initWidgets();

    QString welcome = tr("Welcome to Gilligan - connect to a network to start "
                         "chatting.");
    statusBar()->showMessage(welcome);

    setWindowTitle(tr("Gilligan Client - Not Connected"));
    setMinimumSize(480, 320);
    resize(480, 320);

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
    QString serverName;

    QString dialogMsg = tr("Enter the name or IP address of a DCP network, "
                           "and Gilligan will try to connect you.");
    serverName = QInputDialog::getText(this, tr("Connect To Server"),
                                       dialogMsg);
    conn = new DCPConnection(this);

    connect(conn, SIGNAL(networkConnected()), this, SLOT(connected()));
    connect(conn, SIGNAL(messageReceived(DCPMessage*)), this, SLOT(received(DCPMessage*)));

    conn->connectTo(serverName);
}



void MainWindow::connected()
{
    output->append("Now connected");
    connectAct->setEnabled(false);
    disconnectAct->setEnabled(true);

    QMultiHash<QString, QString> signon;
    signon.insert("handle", "andrew@foxkit.us");
    signon.insert("password", "arffoxes");
    signon.insert("options", "*");
    conn->sendMessage(new DCPMessage("*", "*", "SIGNON", signon));
}



void MainWindow::received(DCPMessage *message)
{
    output->append("<" + message->source + "> (message)");
}



void MainWindow::disconnect()
{
    delete conn;
    conn = NULL;

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
        command = raw_input.mid(1, cmd_length);

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

    conn->sendMessage(new DCPMessage("*", dest, command, params));
}



/** showSpec - show the current draft spec */
void MainWindow::showSpec()
{
    QDesktopServices::openUrl(QUrl("http://foxkit.us/spec.pdf"));
}



/** about - show information about us */
void MainWindow::about()
{
    QMessageBox::about(this, tr("Gilligan 0.1"), tr("Put stuff here."));
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
    connect(disconnectAct, SIGNAL(triggered()), this, SLOT(close()));

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
    output->setEnabled(false);

    input = new QLineEdit;
    connect(input, SIGNAL(returnPressed()), this, SLOT(sendMessage()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(output);
    layout->addWidget(input);
    central->setLayout(layout);
}
