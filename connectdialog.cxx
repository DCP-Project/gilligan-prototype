#include "connectdialog.h"

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent)
{
    connectButton = new QPushButton(tr("&Connect"));
    connectButton->setDefault(true);

    explanation = new QLabel(tr("Enter your credentials and Gilligan will attempt to connect you to DCP."));

    serverEdit = new QLineEdit;
    handleEdit = new QLineEdit;
    passphraseEdit = new QLineEdit;
    passphraseEdit->setEchoMode(QLineEdit::Password);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("&Server:"), serverEdit);
    formLayout->addRow(tr("&Handle:"), handleEdit);
    formLayout->addRow(tr("&Passphrase:"), passphraseEdit);

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
