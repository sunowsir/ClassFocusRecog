#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "ocsfs_module_trainer.h"
#include "ocsfs_define.h"



/* 训练所用的各个表情存放路径，不需要写前缀路径，只需要写当前的哪个文件夹，文件夹的名字即可，
 * 例如，我在/home/sun/Code/1.github.own/OnlineClassroomStatusFeedbackSystem/bin/comm/ 
 * 路径下存放平静表情的训练图片，那么我下面只需要写/comm即可
 * 注意！！！:
 * 1. 如下列表的第一个路径/comm不叫第一个路径，叫第零个！！！！！
 * 2. 如下路径顺序必须与如上表情编号定义顺序对应，比如
 * 如下第零个路径，它的表情编号应该是: OCSFS_FACE_BASE + 0 * OCSFS_FACE_STEP
 * */
QStringList Trans_Images_Path = {
    "/comm", 
    "/happy", 
    "/hade", 
    "/angry", 
    "/closeeyes", 
    "/yawn"
};


int main(int argc, char *argv[]) {
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

    /* 使用图像训练器训练模型 */
    /* 设置有几种表情类型，以及每种类型的训练图片有多少 */
    OCSFS_Module_Trainer smt(OCSFS_FACE_TYPE_NUM, OCSFS_TRAIN_IMGS_NUM);

    for (int idx = 0; idx < Trans_Images_Path.size(); idx++) {
        int type_id = OCSFS_FACE_BASE + idx * OCSFS_FACE_STEP;
        QString type_img_path = QCoreApplication::applicationDirPath() + Trans_Images_Path.at(idx);
        smt.load_train_data(type_img_path, type_id);
    }

    // smt.load_train_data(QCoreApplication::applicationDirPath() + QString("/comm"), OCSFS_face_COMM);
    // smt.load_train_data(QCoreApplication::applicationDirPath() + QString("/happy"), OCSFS_face_HAPPY);
    // smt.load_train_data(QCoreApplication::applicationDirPath() + QString("/hade"), OCSFS_face_HADE);

    smt.train_module_2_xml();


    for (int idx = 0; idx < Trans_Images_Path.size(); idx++) {
        int type_id = OCSFS_FACE_BASE + idx * OCSFS_FACE_STEP;
        QString type_img_path = QCoreApplication::applicationDirPath() + Trans_Images_Path.at(idx);
        smt.train_module_test(type_img_path, type_id);
    }

    // smt.train_module_test(QCoreApplication::applicationDirPath() + QString("/comm"), OCSFS_face_COMM);
    // smt.train_module_test(QCoreApplication::applicationDirPath() + QString("/happy"), OCSFS_face_HAPPY);
    // smt.train_module_test(QCoreApplication::applicationDirPath() + QString("/hade"), OCSFS_face_HADE);

    return 0;
}
