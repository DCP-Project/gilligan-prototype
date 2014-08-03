#ifndef REGISTRATIONWIZARD_PRIVATE_H
#define REGISTRATIONWIZARD_PRIVATE_H

#include <QWizardPage>
#include <QRadioButton>
#include <QLineEdit>
#include <QTextEdit>

class RegIntroPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit RegIntroPage(QWidget *parent = 0);

signals:

public slots:

private:
    QRadioButton *useCentral;
    QRadioButton *privateServer;
    QLineEdit *serverName;
};

class RegAccountPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit RegAccountPage(QWidget *parent = 0);

signals:

public slots:

private:
    QLineEdit *handle;
    QLineEdit *passphrase;
};

class RegPersonalPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit RegPersonalPage(QWidget *parent = 0);

signals:

public slots:

private:
    QLineEdit *gecos;
    QLineEdit *clientName;
};

class RegFinishPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit RegFinishPage(QWidget *parent = 0);

signals:

public slots:

private:
    QTextEdit *info;
};

#endif // REGISTRATIONWIZARD_PRIVATE_H
