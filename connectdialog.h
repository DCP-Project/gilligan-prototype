#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#ifdef HAVE_KDE
#include <KWallet/Wallet>

using KWallet::Wallet;
#endif

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
#ifdef HAVE_KDE
    void maybeSave();
    void savedCredentialSelected(int index);
    void walletOpened(bool success);
#endif

private:
    QLabel *explanation;

#ifdef HAVE_KDE
    QComboBox *creds;
#endif

    QLineEdit *serverEdit;
    QLineEdit *handleEdit;
    QLineEdit *passphraseEdit;
    QLineEdit *clientEdit;

#ifdef HAVE_KDE
    QCheckBox *saveCreds;
#endif

    QPushButton *connectButton;

#ifdef HAVE_KDE
    Wallet *wallet;
#endif
};

#endif // CONNECTDIALOG_H
