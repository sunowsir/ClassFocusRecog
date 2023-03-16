/*
	* File     : ssi_face_recognition.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月13日 星期一 20时28分12秒
*/

#include "ssi_face_recognition.h"

SSI_Face_Recognition::SSI_Face_Recognition(cv::Mat &img) {
    this->frame = &img;
}

SSI_Face_Recognition::~SSI_Face_Recognition() {
}

bool SSI_Face_Recognition::recognize() {
    cv::Mat dst;
    
    //提取灰度图
    cv::cvtColor(*frame, dst, CV_BGR2GRAY);

    //加载dlib的人脸识别器
    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();

    //加载人脸形状探测器
    dlib::deserialize(QCoreApplication::applicationDirPath().toStdString() + 
        "/shape_predictor_68_face_landmarks.dat") >> this->sp;

    //Mat转化为dlib的matrix
    dlib::assign_image(this->dimg, dlib::cv_image<uchar>(dst)); 

    //获取一系列人脸所在区域
    this->faces = detector(dimg);
    std::cout << "Number of faces detected: " << faces.size() << std::endl;

    if (this->faces.size() == 0)
        return false;

    //获取人脸特征点分布
    for (int i = 0; i < this->faces.size(); i++) {
        dlib::full_object_detection shape = sp(dimg, faces[i]); //获取指定一个区域的人脸形状
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
