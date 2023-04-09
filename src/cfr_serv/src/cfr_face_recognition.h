/*
	* File     : cfr_face_recognition.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月13日 星期一 20时28分08秒
*/

#ifndef _CFR_FACE_RECOGNITION_H
#define _CFR_FACE_RECOGNITION_H

#include <dlib/opencv.h>
#include <dlib/image_io.h>
#include <opencv2/opencv.hpp>
#include <dlib/gui_widgets.h>
#include <opencv2/imgproc/types_c.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing/shape_predictor.h>

#include <QDir>
#include <QImage>
#include <QCoreApplication>


/* 人脸识别器 */
class CFR_Face_Recognition {
private: 
    /* 人脸形状探测器 */
    dlib::shape_predictor sp;

    /* dlib 人脸识别器 */
    dlib::frontal_face_detector detector;

    /* haar 人脸检测器(haar级联分类器), 与detector二选一 */
    cv::CascadeClassifier faceDetector;

private: 
    bool dlib_recognize(cv::Mat&, std::vector<dlib::rectangle>&, std::vector<dlib::full_object_detection>&);
    bool cv_recognize(cv::Mat&, std::vector<dlib::rectangle>&, std::vector<dlib::full_object_detection>&);

public: 
    CFR_Face_Recognition(const QString&, const QString&);
    ~CFR_Face_Recognition();

    bool recognize(cv::Mat&, std::vector<dlib::rectangle>&, std::vector<dlib::full_object_detection>&);
};

#endif
