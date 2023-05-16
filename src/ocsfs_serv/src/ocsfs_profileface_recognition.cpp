/*
	* File     : ocsfs_profileface_recognition.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月08日 星期一 20时21分24秒
*/

#include "ocsfs_profileface_recognition.h"

OCSFS_Profileface_Recognition::OCSFS_Profileface_Recognition(const QString &md_file_path) {
    if (!this->faceDetector.load(md_file_path.toStdString())) 
        qDebug() << "Failed to load face detector.";
}

OCSFS_Profileface_Recognition::~OCSFS_Profileface_Recognition() {
}

bool OCSFS_Profileface_Recognition::recognize(const QImage& img) {
    cv::Mat frame;
    OCSFS_Image_Converter sic;
    if (!sic.qimage_2_mat(img, frame)) {
        qDebug() << "convert QImage to img failed.";
        return false;
    }

    cv::Mat gray;
    
    /* 提取灰度图 */
    cv::cvtColor(frame, gray, CV_BGR2GRAY);

    /* 直方图均衡化 */
    cv::equalizeHist(gray, gray);   

    std::vector<cv::Rect> cv_faces;

    /* 检测人脸，使用haarcascade_profileface.xml识别到即侧脸，否则非侧脸 */
    this->faceDetector.detectMultiScale(gray, cv_faces);
    if (cv_faces.size() > 0) 
        return true;

    /* 若识别不到，镜像翻转图片再次识别 */
    cv::Mat flipped;
    flip(gray, flipped, 1);

    /* 检测镜像人脸，使用haarcascade_profileface.xml识别到即侧脸，否则非侧脸 */
    this->faceDetector.detectMultiScale(flipped, cv_faces);
    if (cv_faces.size() <= 0) 
        return false;
    
    return true;
}
