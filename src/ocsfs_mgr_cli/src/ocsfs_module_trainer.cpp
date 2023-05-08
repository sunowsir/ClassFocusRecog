/*
	* File     : ocsfs_module_trainer.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月07日 星期二 19时44分57秒
*/

#include "ocsfs_module_trainer.h"

bool OCSFS_Module_Trainer::capture_and_save_keypoint(cv::Mat& frame, int& trans_mat_row) {
    /* 一系列人脸所在区域 */
    std::vector<dlib::rectangle> faces;

    /* 人脸特征点分布 */
    std::vector<dlib::full_object_detection> shapes;

    /* 开始识别 */
    if (false == this->sfr->recognize(frame, faces, shapes)) {
        qDebug() << "sfr.recognize failed.";
        return false;
    }

    /* 系数 */
    float offset = -(faces[0].top() - faces[0].bottom()) / (float)OCSFS_FACE_MAX;

    /* 一个图片有68个特征值，每个特征值能得到两个特征向量 */
    for (int i = 0; i < 68; i++) {
        float kp_offset = (shapes[0].part(i).x() - faces[0].left()) / offset;
        float* pixel_ptr = this->trans_mat->ptr<float>(trans_mat_row, i * 2);
        *pixel_ptr = kp_offset;

        kp_offset = (shapes[0].part(i).y() - faces[0].top()) / offset;
        pixel_ptr = this->trans_mat->ptr<float>(trans_mat_row, i * 2 + 1);
        *pixel_ptr = kp_offset;
    }

    return true;
}

OCSFS_Module_Trainer::OCSFS_Module_Trainer(int tnum, int inum) {
    if (tnum <= 0) {
        qDebug() << "type_num <= 0";
        return ;
    }
    if (inum <= 0) {
        qDebug() << "img_num <= 0";
        return ;
    }

    this->type_num = tnum;
    this->img_num = inum;

    int row = (this->type_num * this->img_num);

    this->trans_mat = new cv::Mat(row, (68 * 2), CV_32FC1);
    this->trans_label = new cv::Mat(row, 1, CV_32SC1);

    this->sfr = new OCSFS_Face_Recognition(QCoreApplication::applicationDirPath() + 
        QString("/shape_predictor_68_face_landmarks.dat"), 
        QCoreApplication::applicationDirPath() + 
        QString("/haarcascade_frontalface_alt.xml"));

    return ;
}

OCSFS_Module_Trainer::~OCSFS_Module_Trainer() {
    delete this->trans_mat;
    delete this->trans_label;
    delete this->sfr;

    return ;
}

bool OCSFS_Module_Trainer::load_train_data(const QString& img_path, 
    const int& face_type) {
    if (face_type < OCSFS_FACE_BASE) {
        qDebug() << "face_type < OCSFS_FACE_BASE";
        return false;
    }
    if (QString("") == img_path) {
        qDebug() << "img_path is null";
        return false;
    }

    int row = (this->type_num * this->img_num);

    int img_idx_start = (face_type - OCSFS_FACE_BASE) / OCSFS_FACE_STEP  * this->img_num;

    for (int i = img_idx_start; i < img_idx_start + this->img_num; i++) {
        int* pixel_ptr = this->trans_label->ptr<int>(i);
        *pixel_ptr = face_type;
    }

    switch(face_type) {
        case OCSFS_face_COMM: {
            std::cout << "平静";
        } break;
        case OCSFS_face_HAPPY: {
            std::cout << "开心";
        } break;
        case OCSFS_face_HADE: {
            std::cout << "厌恶";
        } break;
        default: {
            std::cout << "unknow";
        }
    }
    std::cout << ": {" << std::endl;

    int img_idx = img_idx_start;

    /* 遍历目录中的所有文件 (code from ChatGPT) */
    for (const auto & entry : ns_fs::directory_iterator(img_path.toStdString())) {
        if (img_idx - img_idx_start >= this->img_num) 
            break;

        /* 检查文件是否为图像 */
        if ((".jpg" != entry.path().extension()) &&
            (".png" != entry.path().extension())) 
            continue;

        std::cout << "载入第" << img_idx << "张: " << entry.path().string() << std::endl;

        /* 将图像文件读入 Mat 对象 */
        cv::Mat frame = cv::imread(entry.path().string());

        /* 在此处对图像进行处理 */
        if (false == this->capture_and_save_keypoint(frame, img_idx)) {
            qDebug() << "image_identification false";
            continue;
        }
        
        img_idx++;
    }
    std::cout << "};" << std::endl;
    
    return true;
}

bool OCSFS_Module_Trainer::train_module_2_xml() {
    int row = (this->type_num * this->img_num);

    /* 构造svm对象 */
    cv::Ptr<ns_CVML::SVM> svm = ns_CVML::SVM::create();

    /* 设置svm对象使用C-SVM（C-Support Vector Machine）模型 */
    svm->setType(ns_CVML::SVM::C_SVC);

    /* 设置svm对象使用线性核 */
    svm->setKernel(ns_CVML::SVM::LINEAR);

    /* 设置svm对象的训练终止条件
     * MAX_ITEM表示最大迭代次数，即在达到该次数后，SVM的训练将自动终止
     * 100表示精度阈值，当SVM的训练误差小于该阈值时，训练将自动终止；
     * 1e-6则表示惩罚系数,
     * * 惩罚系数控制了模型的过拟合程度。
     * * * 当惩罚系数较小时，模型容易出现过拟合现象；
     * * * 而当惩罚系数较大时，则容易出现欠拟合现象*/
    svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));

    /* 开始训练，
     * this->trans_mat表示训练数据矩阵，每一行代表一个样本，每一列代表该样本的一个特征；
     * ROW_SAMPLE参数表示样本矩阵的每一行代表一个样本，即每个样本都以行向量的形式出现
     * this->trans_label则表示训练数据对应的标签向量，即每个样本所属的类别*/
    svm->train(*this->trans_mat, ns_CVML::ROW_SAMPLE, *this->trans_label);

    std::string xml_fname = 
        std::string(QCoreApplication::applicationDirPath().toStdString() + std::string("/SVM_DATA.xml"));

    std::cout << "训练完成：" << xml_fname << std::endl;

    /* 保存模型到文件 */
    svm->save(xml_fname);

    qDebug() << "over!!!";

    return true;
}

/* 模型测试 */
bool OCSFS_Module_Trainer::train_module_test(const QString& img_path, 
    const int& expected_face_type) {

    /* 表情识别器 */
    OCSFS_Expression_Recognition ser(QCoreApplication::applicationDirPath() + QString("/SVM_DATA.xml"));

    int img_idx = (expected_face_type - OCSFS_FACE_BASE) / OCSFS_FACE_STEP  * this->img_num;
    int face_type = 0;

    std::cout << "使用训练图片进行模型识别测试: " << std::endl;

    switch(expected_face_type) {
        case OCSFS_face_COMM: {
            std::cout << "平静";
        } break;
        case OCSFS_face_HAPPY: {
            std::cout << "开心";
        } break;
        case OCSFS_face_HADE: {
            std::cout << "厌恶";
        } break;
        default: {
            std::cout << "unknow";
        }
    }
    std::cout << ": {" << std::endl;
    for (const auto & entry : ns_fs::directory_iterator(img_path.toStdString())) {
        /* 检查文件是否为图像 */
        if ((".jpg" != entry.path().extension()) &&
            (".png" != entry.path().extension())) {
            continue;
        }

        std::cout << "载入第" << img_idx++ << "张: " << entry.path().string() << std::endl;

        /* 将图像文件读入 Mat 对象 */
        // cv::Mat frame = cv::imread(entry.path().string());
        QImage frame;
        frame.load(QString::fromStdString(entry.path()));

        ser.recognize(frame, frame, face_type);

        std::cout << "预期：";
        switch(expected_face_type) {
            case OCSFS_face_COMM: {
                std::cout << "平静";
            } break;
            case OCSFS_face_HAPPY: {
                std::cout << "开心";
            } break;
            case OCSFS_face_HADE: {
                std::cout << "厌恶";
            } break;
            default: {
                std::cout << "unknow";
            }
        }
        std::cout << "(" << expected_face_type << "), 识别结果: ";

        switch(face_type) {
            case OCSFS_face_COMM: {
                std::cout << "平静";
            } break;
            case OCSFS_face_HAPPY: {
                std::cout << "开心";
            } break;
            case OCSFS_face_HADE: {
                std::cout << "厌恶";
            } break;
            default: {
                std::cout << "unknow";
            }
        }
        std::cout << "(" << face_type << ");" << std::endl;
        
    }
    std::cout << "};" << std::endl;
    
    return true;
}
