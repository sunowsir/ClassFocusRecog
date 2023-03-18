/*
	* File     : ssi_module_trainer.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月07日 星期二 19时44分57秒
*/

#include "ssi_module_trainer.h"

bool SSI_Module_Trainer::capture_and_save_keypoint(cv::Mat& frame, float *kp_offset_arr) {
    /* 开始识别 */
    if (false == this->sfr->recognize(frame)) {
        qDebug() << "sfr.recognize failed.";
        return false;
    }

    /* 一系列人脸所在区域 */
    std::vector<dlib::rectangle> &faces = this->sfr->faces_get();

    /* 人脸特征点分布 */
    std::vector<dlib::full_object_detection> &shapes = this->sfr->shapes_get();

    /* 系数 */
    float offset = -(faces[0].top() - faces[0].bottom());

    int kp_offset_arr_num = 0;
    for (int j = 0; j < 68; j++) {

        float kp_offset = shapes[0].part(j).x() - faces[0].left()  / offset;
        if (nullptr != kp_offset_arr)
            kp_offset_arr[kp_offset_arr_num++] = kp_offset;

        kp_offset = shapes[0].part(j).y() - faces[0].left()  / offset;
        if (nullptr != kp_offset_arr)
            kp_offset_arr[kp_offset_arr_num++] = kp_offset;
    }

    return true;
}

SSI_Module_Trainer::SSI_Module_Trainer(int tnum, int inum) {

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

    this->trans_kp_arr = (float **)malloc(sizeof(float*) * row);
    for (int i = 0; i < row; i++) {
        this->trans_kp_arr[i] = (float*)malloc(sizeof(float) * (68 * 2));
        memset(this->trans_kp_arr[i], 0, sizeof(float) * (68 * 2));
    }

    this->face_label = (int*)malloc(sizeof(int) * row);
    memset(this->face_label, 0, sizeof(int) * row);

    this->sfr = new SSI_Face_Recognition(QCoreApplication::applicationDirPath() + 
        QString("/shape_predictor_68_face_landmarks.dat"));

    return ;
}

SSI_Module_Trainer::~SSI_Module_Trainer() {

    int row = (this->type_num * this->img_num);
    for (int i = 0; i < row; i++) {
        free(this->trans_kp_arr[i]);
        this->trans_kp_arr[i] = nullptr;
    }
    free(this->trans_kp_arr);
    this->trans_kp_arr = nullptr;

    free(this->face_label);

    return ;
}


bool SSI_Module_Trainer::load_train_data(const QString& img_path, 
    const int& face_type) {
    if (face_type < SSI_FACE_BASE) {
        qDebug() << "face_type < SSI_FACE_BASE";
        return false;
    }
    if (QString("") == img_path) {
        qDebug() << "img_path is null";
        return false;
    }

    int row = (this->type_num * this->img_num);

    for (int i = 0; i < this->img_num; i++) {
        this->face_label[i + ((face_type - SSI_FACE_BASE) * this->img_num)] = face_type;
    }

    int img_idx = (face_type - SSI_FACE_BASE) * this->img_num;
    /* 遍历目录中的所有文件 (code from ChatGPT) */
    for (const auto & entry : ns_fs::directory_iterator(img_path.toStdString())) {
        if (img_idx - (face_type - SSI_FACE_BASE) * this->img_num >= this->img_num)
            break;

        /* 检查文件是否为图像 */
        if (entry.path().extension() == ".jpg" || 
            entry.path().extension() == ".png") {

            std::cout << entry.path().string() << std::endl;

            /* 将图像文件读入 Mat 对象 */
            cv::Mat frame = cv::imread(entry.path().string());

            /* 在此处对图像进行处理 */
            if (false == this->capture_and_save_keypoint(frame, this->trans_kp_arr[img_idx++])) {
                qDebug() << "image_identification false";
                continue;
            }
        
        }
    }
    
    return true;
}

bool SSI_Module_Trainer::train_module_2_xml() {
    int row = (this->type_num * this->img_num);

    cv::Mat train_mat(row, (2 * 68), CV_32FC1, this->trans_kp_arr);
    cv::Mat label_mat(row, 1, CV_32SC1, this->face_label);

    cv::Ptr<ns_CVML::SVM> svm = ns_CVML::SVM::create();
    svm->setType(ns_CVML::SVM::C_SVC);
    svm->setKernel(ns_CVML::SVM::LINEAR);
    svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1E-6));

    svm->train(train_mat, ns_CVML::ROW_SAMPLE, label_mat);

    std::string xml_fname = std::string(QCoreApplication::applicationDirPath().toStdString() + 
        std::string("/SVM_DATA.xml"));
    svm->save(xml_fname);

    qDebug() << "over!!!";

    return true;
}
