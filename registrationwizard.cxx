#include "registrationwizard.h"
#include "registrationwizard_private.h"

#include <QFormLayout>
#include <QHostInfo>

RegistrationWizard::RegistrationWizard(QWidget *parent) :
    QWizard(parent)
{
    setPage(Page_Intro, new RegIntroPage);
    setPage(Page_Account, new RegAccountPage);
    setPage(Page_Personal, new RegPersonalPage);
    setPage(Page_Finish, new RegFinishPage);
}

QString RegistrationWizard::clientName()
{
    QString name = field("clientName").toString();

    if(name.length() == 0) name = QHostInfo::localHostName();

    return name;
}

RegIntroPage::RegIntroPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Welcome to DCP!"));
    setSubTitle(tr("This wizard will help you start chatting on DCP in three simple steps."));

    useCentral = new QRadioButton(tr("Register with the central DCP server."), this);
    privateServer = new QRadioButton(tr("Register on a private server."), this);

    serverName = new QLineEdit;
    serverName->setVisible(false);
    registerField("serverName", serverName);

    connect(privateServer, SIGNAL(toggled(bool)), serverName, SLOT(setVisible(bool)));

    QFormLayout *layout = new QFormLayout;
    layout->addWidget(useCentral);
    layout->addWidget(privateServer);
    layout->addRow(tr(""), serverName);

    setLayout(layout);
}

RegAccountPage::RegAccountPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Enter your account information."));
    setSubTitle(tr("Pick the handle you want.  You can't change it later, so "
                   "be sure you're happy with it.  Also pick a good, secure "
                   "passphrase and store it in a safe place."));

    handle = new QLineEdit;
    passphrase = new QLineEdit;
    passphrase->setEchoMode(QLineEdit::Password);

    registerField("handle*", handle);
    registerField("passphrase*", passphrase);

    QFormLayout *layout = new QFormLayout;
    layout->addRow(tr("Handle:"), handle);
    layout->addRow(tr("Passphrase:"), passphrase);

    setLayout(layout);
}

void RegAccountPage::cleanupPage()
{
    passphrase->setText("");
}

RegPersonalPage::RegPersonalPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Enter your personal information."));
    setSubTitle(tr("Provide your real name to help personalise your DCP "
                   "experience.  Specify the name of this computer to help you "
                   "distinguish it from other computers you may use."));

    gecos = new QLineEdit;
    clientName = new QLineEdit;
    clientName->setPlaceholderText(QHostInfo::localHostName());

    registerField("gecos*", gecos);
    registerField("clientName", clientName);

    QFormLayout *layout = new QFormLayout;
    layout->addRow(tr("Your name:"), gecos);
    layout->addRow(tr("Computer name:"), clientName);

    setLayout(layout);
}

RegFinishPage::RegFinishPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Verify your information."));
    setSubTitle(tr("That's it!  Please review your information below.  If you "
                   "are happy, click Finish and you will be registered on DCP. "
                   "Happy chatting :)"));

    info = new QTextEdit;
    info->setReadOnly(true);

    QFormLayout *layout = new QFormLayout;
    layout->addWidget(info);

    setLayout(layout);
}

void RegFinishPage::initializePage()
{
    info->setHtml("<h3>Account Information</h3><dl><dt>Handle:</dt><dd>"
                  + field("handle").toString() + "</dd></dl>" + "<hr>" +
                  "<h3>Personal Information</h3><dl><dt>Name:</dt><dd>"
                  + field("gecos").toString() + "</dd></dl>");
}
