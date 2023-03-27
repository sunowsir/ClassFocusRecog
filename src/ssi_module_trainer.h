/*
	* File     : ssi_module_trainer.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月07日 星期二 19时44分59秒
*/

#ifndef _SSI_MODULE_TRAINER_H
#define _SSI_MODULE_TRAINER_H

#include <ctime>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>


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

#include "ssi_face_recognition.h"
#include "ssi_expression_recognition.h"

namespace ns_fs = std::filesystem;
namespace ns_CVML = cv::ml;


/* 模型训练器 */
class SSI_Module_Trainer {
private: 

    /* 人脸形状探测器 */
    dlib::shape_predictor sp;

    /* dlib的matrix */
    dlib::array2d<dlib::bgr_pixel> dimg;

    /* 处理后的特征值数据集集 */
    cv::Mat *trans_mat;

    /* 表情标签数据集 */
    cv::Mat *trans_label;

    /* 用于保存训练系数数组的行数 */
    unsigned int type_num;

    /* 用于保存训练系数数组的列数 */
    unsigned int img_num;

    /* 人脸识别器 */
    SSI_Face_Recognition *sfr;

private: 
    /* 采集特征点，并根据特征点计算特征向量，保存特征向量到数据集中 */
    bool capture_and_save_keypoint(cv::Mat&, int&);
public: 

    SSI_Module_Trainer(int /* type num */, int /* img num */);
    ~SSI_Module_Trainer();

    /* 载入训练图片，以及该组图片对应的表情 */
    bool load_train_data(const QString& /* image path */, 
        const int& /* 类别名称 */);

    /* 训练模型，并导出为xml模型文件 */
    bool train_module_2_xml();

    /* 模型测试 */
    bool train_module_test(const QString& /* image path */, 
        const int&/* 预期表情类型 */);
};

#endif
