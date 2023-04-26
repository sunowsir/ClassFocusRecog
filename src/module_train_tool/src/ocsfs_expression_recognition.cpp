/*
	* File     : ocsfs_expression_recognition.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月13日 星期一 21时11分41秒
*/

#include "ocsfs_expression_recognition.h"

OCSFS_Expression_Recognition::OCSFS_Expression_Recognition(const QString& module_filepath) {
    this->svm = ns_CVML::StatModel::load<ns_CVML::SVM>(module_filepath.toStdString());
    this->sfr = new OCSFS_Face_Recognition(QCoreApplication::applicationDirPath() + 
        QString("/shape_predictor_68_face_landmarks.dat"), 
        QCoreApplication::applicationDirPath() + 
        QString("/haarcascade_frontalface_alt.xml"));
}

OCSFS_Expression_Recognition::~OCSFS_Expression_Recognition() {
    delete this->sfr;
}

/* 表情识别，传入图片，获得表情 */
bool OCSFS_Expression_Recognition::recognize(const QImage& img, int& face_type) {
    OCSFS_Image_Converter sic;
    cv::Mat frame;
    if (!sic.qimage_2_mat(img, frame)) {
        qDebug() << "convert QImage to img failed.";
        return false;
    }

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
    
    cv::Mat query_mat(1, (68 * 2), CV_32FC1);

    for (int i = 0; i < 68; i++) {

        float kp_offset = (shapes[0].part(i).x() - faces[0].left()) / offset;
        float* pixel_ptr = query_mat.ptr<float>(0, i * 2);
        *pixel_ptr = kp_offset;
        
        kp_offset = (shapes[0].part(i).y() - faces[0].top()) / offset;
        pixel_ptr = query_mat.ptr<float>(0, i * 2 + 1);
        *pixel_ptr = kp_offset;
    }

    /* 查询结果 */
    face_type = (int)this->svm->predict(query_mat);

    return true;
}
