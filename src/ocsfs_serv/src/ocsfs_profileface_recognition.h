/*
	* File     : ocsfs_profileface_recognition.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月08日 星期一 20时21分26秒
*/

#ifndef _OCSFS_PROFILEFACE_RECOGNITION_H
#define _OCSFS_PROFILEFACE_RECOGNITION_H

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

#include "ocsfs_image_converter.h"

/* 侧脸识别 */
class OCSFS_Profileface_Recognition {
private: 
    /* haar 人脸检测器(haar级联分类器) */
    cv::CascadeClassifier faceDetector;

public: 
    OCSFS_Profileface_Recognition(const QString& /* 分类器 */);
    ~OCSFS_Profileface_Recognition();

    bool recognize(const QImage&);
};

#endif
