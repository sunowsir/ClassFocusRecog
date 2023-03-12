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

namespace ns_fs = std::filesystem;
namespace ns_CVML = cv::ml;


#define SII_FACE_BASE       100
#define SII_face_COMM       SII_FACE_BASE + 0
#define SII_face_HAPPY      SII_FACE_BASE + 1
#define SII_face_HADE       SII_FACE_BASE + 2

class SSI_Img_Idf {
private: 

    /* 人脸形状探测器 */
    dlib::shape_predictor sp;

    /* dlib的matrix */
    dlib::array2d<dlib::bgr_pixel> dimg;

    /* 一系列人脸所在区域 */
    std::vector<dlib::rectangle> dets;

    /* 人脸特征点分布 */
    std::vector<dlib::full_object_detection> shapes;

    /* 系数数组  */
    std::vector<float> *kp_offsets;

    /* 训练得到的系数数组 */
    float **trans_kp_arr;

    /* 表情标签数组 */
    int *face_label;

    /* 用于保存训练系数数组的行数 */
    unsigned int type_num;

    /* 用于保存训练系数数组的列数 */
    unsigned int img_num;

private: 

    bool idf_core(cv::Mat &);

    /* 框出每个脸，标出每个脸中的68个特征点 */
    bool point_face_detections (cv::Mat&);

    /* 采集并保存特征点 */
    bool capture_and_save_keypoint(float* = nullptr);

    /* 写入特征点到文件 */
    bool write_keypoint_2_file(std::string&, std::string&, bool);

public: 

    cv::Mat qimage_2_mat(const QImage&);
    QImage mat_2_qimage(const cv::Mat&);

    SSI_Img_Idf();
    ~SSI_Img_Idf();

    /* 图像识别api */
    QImage image_identification(const QImage& img);

    /* 初始化保存训练得到的系数的数组 */
    bool train_arr_set(int /* type num */, int /* img num */);

    /* 载入训练图片，以及该组图片对应的表情 */
    bool load_train_data(const QString& /* image path */, 
        const int& /* 类别名称 */);

    /* 训练模型，并导出为xml模型文件 */
    bool train_module_2_xml();
};

#endif
