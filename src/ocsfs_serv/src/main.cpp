#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>

#include "ocsfs_server.h"

int main(int argc, char *argv[])
{
    QCoreApplication start(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "OCSFS_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            start.installTranslator(&translator);
            break;
        }
    }

    OCSFS_Server serv;

    return start.exec();
}
