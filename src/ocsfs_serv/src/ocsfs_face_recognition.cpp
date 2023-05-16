/*
	* File     : ocsfs_face_recognition.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月13日 星期一 20时28分12秒
*/

#include "ocsfs_face_recognition.h"

OCSFS_Face_Recognition::OCSFS_Face_Recognition(const QString &sp_fpath, const QString &fd_fpath) {
    /* 加载人脸形状探测器 */
    dlib::deserialize(sp_fpath.toStdString()) >> this->sp;

    this->detector = dlib::get_frontal_face_detector();

    if (!this->faceDetector.load(fd_fpath.toStdString())) 
        qDebug() << "Failed to load face detector.";
}

OCSFS_Face_Recognition::~OCSFS_Face_Recognition() {
}

bool OCSFS_Face_Recognition::dlib_recognize(cv::Mat &frame, 
    std::vector<dlib::rectangle>& dlib_faces, 
    std::vector<dlib::full_object_detection>& shapes) {
    cv::Mat dst;
    
    /* 提取灰度图 */
    cv::cvtColor(frame, dst, CV_BGR2GRAY);

    /* dlib的matrix */
    dlib::array2d<dlib::bgr_pixel> dimg;

    /* Mat转化为dlib的matrix */
    dlib::assign_image(dimg, dlib::cv_image<uchar>(dst)); 

    /* 获取一系列人脸所在区域 */
    dlib_faces = this->detector(dimg);
    std::cout << "Number of faces detected: " << dlib_faces.size() << std::endl;

    if (0 == dlib_faces.size()) {
        qDebug() << "no faces";
        return false;
    }

    /* 获取人脸特征点分布 */
    for (int i = 0; i < dlib_faces.size(); i++) {
        /* 获取指定一个区域的人脸形状 */
        dlib::full_object_detection shape = this->sp(dimg, dlib_faces[i]); 
        shapes.push_back(shape); 
    }   
    if (shapes.empty()) {
        qDebug() << "shapes is empty";
        return false;
    }

    return true;
}

bool OCSFS_Face_Recognition::cv_recognize(cv::Mat &frame, 
    std::vector<dlib::rectangle>& dlib_faces, 
    std::vector<dlib::full_object_detection>& shapes) {
    cv::Mat gray;
    
    /* 提取灰度图 */
    cv::cvtColor(frame, gray, CV_BGR2GRAY);

    /* 直方图均衡化 */
    cv::equalizeHist(gray, gray);   

    std::vector<cv::Rect> cv_faces;

    /* 检测人脸 */
    this->faceDetector.detectMultiScale(gray, cv_faces);
    if (0 == cv_faces.size()) 
        qDebug() << "no face";

    /* dlib的matrix */
    dlib::array2d<dlib::bgr_pixel> dimg;

    /* Mat转化为dlib的matrix */
    dlib::assign_image(dimg, dlib::cv_image<uchar>(gray)); 

    /* 转为dlib face */
    for (const auto& face : cv_faces) {
        dlib::rectangle rect(face.x, face.y, face.x + face.width, face.y + face.height);
        dlib_faces.push_back(rect);
    }

    /* 获取人脸特征点分布 */
    // for (int i = 0; i < this->dlib_faces.size(); i++) {
    for (const auto& dlib_face: dlib_faces) {
        /* 获取指定一个区域的人脸形状 */
        dlib::full_object_detection shape = this->sp(dimg, dlib_face); 
        shapes.push_back(shape); 

        // for (int i = 0; i < 68; i++) {
        //     circle(frame, cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 2, cv::Scalar(255, 0, 0), -1);
        // }
    }
    // cv::imwrite("./1.jpg", frame);

    if (shapes.empty()) {
        qDebug() << "shapes is empty";
        return false;
    }
    
    return true;
}

bool OCSFS_Face_Recognition::recognize(cv::Mat &frame, 
    std::vector<dlib::rectangle>& faces, 
    std::vector<dlib::full_object_detection>& shape) {
    return this->cv_recognize(frame, faces, shape);
}

