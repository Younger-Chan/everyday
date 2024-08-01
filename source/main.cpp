#include "everyday.h"
#include "trayicon.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "everyday_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    // QIcon icon(":/other/other/evy.png");
    // a.setWindowIcon(icon);
    EveryDay w;
    trayIcon trayIcon(&w);
    w.show();
    return a.exec();
}
