#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "cfr_window.h"

int main(int argc, char *argv[])
{
    QApplication start(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "CFR_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            start.installTranslator(&translator);
            break;
        }
    }

    CFR_Window cfr_window;
    cfr_window.show();

    return start.exec();
}
