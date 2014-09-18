#include "mainwindow.h"

#ifdef HAVE_KDE
#   include <KApplication>
#   include <KAboutData>
#   include <KCmdLineArgs>
#else
#   include <QApplication>
#endif

#include <QTextCodec>

#ifdef HAVE_KDE
static KAboutData about(
        "Gilligan",
        NULL,
        ki18n("Gilligan"),
        "0.1",
        ki18n("DCP Chat Client (prototype)"),
        KAboutData::License_BSD,
        ki18n("© 2014"),
        KLocalizedString(),
        "http://dcp.foxkit.us/",
        "not yet available");
#endif

int main(int argc, char *argv[])
{
#ifdef HAVE_KDE
    about.addAuthor(ki18n("Andrew Wilcox"), ki18n("Developer"),
                    "awilcox@wilcox-tech.com", "http://foxkit.us/");
    about.addAuthor(ki18n("Elizabeth Myers"), ki18n("Protocol Development"),
                    "elizabeth@interlinked.me");

    KCmdLineArgs::init(argc, argv, &about);
    KApplication app;
#else
    QApplication app(argc, argv);
#endif

    // All DCP strings are UTF-8, per spec.
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    MainWindow w;
    w.show();

    return app.exec();
}
