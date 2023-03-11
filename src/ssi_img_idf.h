/*
	* File     : ssi_img_idf.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月07日 星期二 19时44分59秒
*/

#ifndef _SSI_IMG_IDF_H
#define _SSI_IMG_IDF_H

#include <ctime>
#include <vector>
#include <iostream>


#include <dlib/opencv.h>
#include <dlib/image_io.h>
#include <opencv2/opencv.hpp>
#include <dlib/gui_widgets.h>
#include <opencv2/imgproc/types_c.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>

#include <QImage>
#include <QCoreApplication>


class SSI_Img_Idf {
private: 

    cv::Mat frame;

    /* 人脸形状探测器 */
    dlib::shape_predictor sp;

    /* dlib的matrix */
    dlib::array2d<dlib::bgr_pixel> dimg;

    /* 一系列人脸所在区域 */
    std::vector<dlib::rectangle> dets;

    /* 人脸特征点分布 */
    std::vector<dlib::full_object_detection> shapes;

    bool idf_core();
    void point_face_detections ();
    void capture_and_save_keypoint();

public: 

    cv::Mat qimage_2_mat(const QImage&);
    QImage mat_2_qimage(const cv::Mat&);

    SSI_Img_Idf();
    ~SSI_Img_Idf();

    QImage image_identification(const QImage& img);
};

#endif
