#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "ssi_window.h"

int main(int argc, char *argv[])
{
    QApplication start(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "SSI_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            start.installTranslator(&translator);
            break;
        }
    }

    SSI_Window ssi_window;
    ssi_window.show();

    return start.exec();
}
