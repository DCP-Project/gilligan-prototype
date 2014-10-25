#include "registrationwizard.h"
#include "registrationwizard_private.h"

#include <QHostInfo>

RegistrationWizard::RegistrationWizard(QWidget *parent) :
    QWizard(parent)
{
    setPage(Page_Intro, new RegIntroPage);
    setPage(Page_Account, new RegAccountPage);
    setPage(Page_Personal, new RegPersonalPage);
    setPage(Page_Finish, new RegFinishPage);

    setWindowTitle(tr("New DCP Account Wizard"));
}

QString RegistrationWizard::clientName()
{
    QString name = field("clientName").toString();

    if(name.length() == 0) name = QHostInfo::localHostName();

    return name;
}
