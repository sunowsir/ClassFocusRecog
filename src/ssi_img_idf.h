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

    bool idf_core(cv::Mat&);
    void line_one_face_detections (cv::Mat img, std::vector<dlib::full_object_detection> fs);

public: 

    cv::Mat qimage_2_mat(const QImage&);
    QImage mat_2_qimage(const cv::Mat&);

    SSI_Img_Idf();
    ~SSI_Img_Idf();

    QImage image_identification(const QImage& img);
};

#endif
