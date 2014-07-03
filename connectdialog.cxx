#include "connectdialog.h"

#include <QtGui>
#include <QHostInfo>

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent)
{
    connectButton = new QPushButton(tr("&Connect"));
    connectButton->setDefault(true);
    connect(connectButton, SIGNAL(clicked()), this, SLOT(maybeSave()));

    explanation = new QLabel(tr("Enter your credentials and Gilligan will attempt to connect you to DCP."));

    creds = new QComboBox;
    creds->setEditable(false);
    creds->setEnabled(false);
    creds->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    connect(creds, SIGNAL(currentIndexChanged(int)), this, SLOT(savedCredentialSelected(int)));

    serverEdit = new QLineEdit;
    handleEdit = new QLineEdit;
    passphraseEdit = new QLineEdit;
    passphraseEdit->setEchoMode(QLineEdit::Password);
    clientEdit = new QLineEdit;
    clientEdit->setPlaceholderText(QHostInfo::localHostName());

    saveCreds = new QCheckBox;
    saveCreds->setText(tr("Save credentials for later"));
    saveCreds->setEnabled(false);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("Sa&ved credentials:"), creds);
    formLayout->addRow(tr("&Server:"), serverEdit);
    formLayout->addRow(tr("&Handle:"), handleEdit);
    formLayout->addRow(tr("&Passphrase:"), passphraseEdit);
    formLayout->addRow(tr("&Client:"), clientEdit);
    formLayout->addWidget(saveCreds);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(connectButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(QDialogButtonBox::Close);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(explanation);
    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);
    setLayout(layout);

    wallet = Wallet::openWallet(Wallet::NetworkWallet(), winId(),
                                Wallet::Asynchronous);
    connect(wallet, SIGNAL(walletOpened(bool)), this, SLOT(walletOpened(bool)));
}

static QString folder = QString("DCP Credentials");

void ConnectDialog::walletOpened(bool success)
{
    if(!success)
        return;     // fail fast, we can't do anything.

    if(!wallet->hasFolder(folder)
            && !wallet->createFolder(folder))
        return;     // we need that folder

    wallet->setFolder(folder);

    // We support saving now.
    saveCreds->setEnabled(true);
    creds->setEnabled(true);

    QStringList list = wallet->entryList();
    QListIterator<QString> iter(list);
    while(iter.hasNext())
    {
        QString cred = iter.next();
        QStringList parts = cred.split('~');
        if(parts.length() != 2) continue;   // invalid

        creds->addItem(QString(parts[0] + " - " + parts[1]), cred);
    }
}

void ConnectDialog::maybeSave()
{
    if(saveCreds->isEnabled() && saveCreds->isChecked())
    {
        QString cred = server() + "~" + handle();
        wallet->writeEntry(cred, passphrase().toUtf8());
    }
}

void ConnectDialog::savedCredentialSelected(int index)
{
    QString cred = creds->itemData(index).toString();

    // get parts of cred
    QStringList parts = cred.split('~');
    if(parts.length() != 2) return;   // invalid

    // read password from wallet
    QByteArray pass;
    wallet->readEntry(cred, pass);

    // set our texts
    serverEdit->setText(parts[0]);
    handleEdit->setText(parts[1]);
    passphraseEdit->setText(QString(pass.constData()));
}

QString ConnectDialog::server()
{
    return serverEdit->text();
}

QString ConnectDialog::handle()
{
    return handleEdit->text();
}

QString ConnectDialog::passphrase()
{
    return passphraseEdit->text();
}

QString ConnectDialog::client()
{
    QString client = clientEdit->text();

    if(client.length() == 0) client = clientEdit->placeholderText();

    return client;
}
