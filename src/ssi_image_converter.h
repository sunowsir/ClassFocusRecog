/*
	* File     : ssi_image_converter.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月13日 星期一 20时48分55秒
*/

#ifndef _SSI_IMAGE_CONVERTER_H
#define _SSI_IMAGE_CONVERTER_H

#include <dlib/opencv.h>
#include <dlib/image_io.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>

#include <QImage>
#include <QCoreApplication>

/* 图片转换器 */
class SSI_Image_Converter {
private: 
    
public: 
    SSI_Image_Converter();
    ~SSI_Image_Converter();

    cv::Mat qimage_2_mat(const QImage&);
    QImage mat_2_qimage(const cv::Mat&);
};

#endif
