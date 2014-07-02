#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QtGui>

class ConnectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConnectDialog(QWidget *parent = 0);

    QString server();
    QString handle();
    QString passphrase();
    QString client();

private:
    QLabel *explanation;

    QLineEdit *serverEdit;
    QLineEdit *handleEdit;
    QLineEdit *passphraseEdit;
    QLineEdit *clientEdit;

    QPushButton *connectButton;
};

#endif // CONNECTDIALOG_H
