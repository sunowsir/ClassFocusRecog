/*
	* File     : ssi_face_recognition.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月13日 星期一 20时28分12秒
*/

#include "ssi_face_recognition.h"

SSI_Face_Recognition::SSI_Face_Recognition(const QString &sp_fpath) {
    //加载人脸形状探测器
    dlib::deserialize(sp_fpath.toStdString()) >> this->sp;
    this->detector = dlib::get_frontal_face_detector();
}

SSI_Face_Recognition::~SSI_Face_Recognition() {
}

bool SSI_Face_Recognition::recognize(cv::Mat &frame) {
    cv::Mat dst;
    
    qDebug() << "1";
    /* 提取灰度图 */
    cv::cvtColor(frame, dst, CV_BGR2GRAY);

    /* dlib的matrix */
    dlib::array2d<dlib::bgr_pixel> dimg;

    qDebug() << "2";

    /* Mat转化为dlib的matrix */
    dlib::assign_image(dimg, dlib::cv_image<uchar>(dst)); 

    qDebug() << "3";
    /* 获取一系列人脸所在区域 */
    this->faces = this->detector(dimg);
    std::cout << "Number of faces detected: " << faces.size() << std::endl;

    if (0 == this->faces.size()) {
        qDebug() << "no faces";
        return false;
    }

    qDebug() << "4";
    /* 获取人脸特征点分布 */
    for (int i = 0; i < this->faces.size(); i++) {
        /* 获取指定一个区域的人脸形状 */
        dlib::full_object_detection shape = this->sp(dimg, faces[i]); 
        this->shapes.push_back(shape); 
    }   
    if (this->shapes.empty()) {
        qDebug() << "shapes is empty";
        return false;
    }

    return true;
}

std::vector<dlib::rectangle>& SSI_Face_Recognition::faces_get() {
    std::vector<dlib::rectangle>& ret = this->faces;
    return ret;
}

std::vector<dlib::full_object_detection>& SSI_Face_Recognition::shapes_get() {
    std::vector<dlib::full_object_detection>& ret = this->shapes;
    return ret;
}
