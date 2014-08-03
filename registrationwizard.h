#ifndef REGISTRATIONWIZARD_H
#define REGISTRATIONWIZARD_H

#include <QWizard>

class RegistrationWizard : public QWizard
{
    Q_OBJECT
public:
    explicit RegistrationWizard(QWidget *parent = 0);

    QString clientName();

signals:

public slots:

private:
    enum { Page_Intro, Page_Account, Page_Personal, Page_Finish };
};

#endif // REGISTRATIONWIZARD_H
