#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "cfr_module_trainer.h"

int main(int argc, char *argv[]) {
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

    /* 使用图像训练器训练模型 */
    /* 设置有几种表情类型，以及每种类型的训练图片有多少 */
    CFR_Module_Trainer smt(3, 50);

    smt.load_train_data(QCoreApplication::applicationDirPath() + QString("/comm"), CFR_face_COMM);
    smt.load_train_data(QCoreApplication::applicationDirPath() + QString("/happy"), CFR_face_HAPPY);
    smt.load_train_data(QCoreApplication::applicationDirPath() + QString("/hade"), CFR_face_HADE);

    smt.train_module_2_xml();

    smt.train_module_test(QCoreApplication::applicationDirPath() + QString("/comm"), CFR_face_COMM);
    smt.train_module_test(QCoreApplication::applicationDirPath() + QString("/happy"), CFR_face_HAPPY);
    smt.train_module_test(QCoreApplication::applicationDirPath() + QString("/hade"), CFR_face_HADE);

    return 0;
}
