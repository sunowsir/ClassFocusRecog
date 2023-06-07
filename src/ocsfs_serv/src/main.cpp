#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>

#include "ocsfs_server.h"

int main(int argc, char *argv[])
{
    std::cout << "1";
    QCoreApplication start(argc, argv);
    std::cout << "2";

    QTranslator translator;
    std::cout << "3";
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    std::cout << "4";
    for (const QString &locale : uiLanguages) {
        const QString baseName = "OCSFS_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            start.installTranslator(&translator);
            break;
        }
    }
    std::cout << "5";

    OCSFS_Server serv;

    std::cout << "6";

    return start.exec();
}
