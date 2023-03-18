/*
	* File     : ssi_expression_recognition.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月13日 星期一 21时11分41秒
*/

#include "ssi_expression_recognition.h"

SSI_Expression_Recognition::SSI_Expression_Recognition(const QString& module_filepath) {
    this->svm = ns_CVML::StatModel::load<ns_CVML::SVM>(module_filepath.toStdString());
    this->sfr = new SSI_Face_Recognition(QCoreApplication::applicationDirPath() + 
        QString("/shape_predictor_68_face_landmarks.dat"));
}

SSI_Expression_Recognition::~SSI_Expression_Recognition() {
}

/* 表情识别，传入图片，获得表情 */
bool SSI_Expression_Recognition::recognize(const QImage& img, int& face_type) {
    SSI_Image_Converter sic;
    cv::Mat frame = sic.qimage_2_mat(img);
    
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

    float testData[1][2 * 68];
    for (int i = 0; i < 68; i++) {
        testData[0][i * 2] = (shapes[0].part(i).x() - faces[0].left()) / offset;
        testData[0][i * 2+1] = (shapes[0].part(i).y() - faces[0].top()) / offset;
    }

    /* 查询结果 */
    cv::Mat query(1, 2 * 68, CV_32FC1, testData);
    face_type = (int)this->svm->predict(query);

    return true;
}
