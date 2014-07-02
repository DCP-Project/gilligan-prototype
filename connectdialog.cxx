#include "connectdialog.h"

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent)
{
    connectButton = new QPushButton(tr("&Connect"));
    connectButton->setDefault(true);
    connect(connectButton, SIGNAL(clicked()), this, SLOT(accept()));

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
