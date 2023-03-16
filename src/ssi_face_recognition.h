/*
	* File     : ssi_face_recognition.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月13日 星期一 20时28分08秒
*/

#ifndef _SSI_FACE_RECOGNITION_H
#define _SSI_FACE_RECOGNITION_H

#include <dlib/opencv.h>
#include <dlib/image_io.h>
#include <opencv2/opencv.hpp>
#include <dlib/gui_widgets.h>
#include <opencv2/imgproc/types_c.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>

#include <QDir>
#include <QImage>
#include <QCoreApplication>


/* 人脸识别器 */
class SSI_Face_Recognition {
private: 
    cv::Mat     *frame;

    /* 一系列人脸所在区域 */
    std::vector<dlib::rectangle> faces;

    /* 人脸形状探测器 */
    dlib::shape_predictor sp;

    /* dlib的matrix */
    dlib::array2d<dlib::bgr_pixel> dimg;

    /* 人脸特征点分布 */
    std::vector<dlib::full_object_detection> shapes;

public: 
    SSI_Face_Recognition(cv::Mat&);
    ~SSI_Face_Recognition();

    bool recognize();
    std::vector<dlib::rectangle>& faces_get();
    std::vector<dlib::full_object_detection>& shapes_get();
};

#endif
