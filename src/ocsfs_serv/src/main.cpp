#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "ocsfs_window.h"

int main(int argc, char *argv[])
{
    QApplication start(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "OCSFS_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            start.installTranslator(&translator);
            break;
        }
    }

    OCSFS_Window ocsfs_window;
    ocsfs_window.show();

    return start.exec();
}
