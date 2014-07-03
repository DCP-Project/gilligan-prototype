#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <KWallet/Wallet>

using KWallet::Wallet;

class ConnectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConnectDialog(QWidget *parent = 0);

    QString server();
    QString handle();
    QString passphrase();
    QString client();

private slots:
    void maybeSave();
    void savedCredentialSelected(int index);
    void walletOpened(bool success);

private:
    QLabel *explanation;

    QComboBox *creds;

    QLineEdit *serverEdit;
    QLineEdit *handleEdit;
    QLineEdit *passphraseEdit;
    QLineEdit *clientEdit;

    QCheckBox *saveCreds;

    QPushButton *connectButton;

    Wallet *wallet;
};

#endif // CONNECTDIALOG_H
