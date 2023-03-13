/*
	* File     : ssi_image_converter.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月13日 星期一 20时49分01秒
*/

#include "ssi_image_converter.h"

SSI_Image_Converter::SSI_Image_Converter() {
}

SSI_Image_Converter::~SSI_Image_Converter() {
}

cv::Mat SSI_Image_Converter::qimage_2_mat(const QImage& image) {
    cv::Mat mat;

    switch (image.format()) {
    case QImage::Format_RGB32:  //一般Qt读入彩色图后为此格式
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);   //转3通道
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    default:
        qDebug() << "QImage to mat failed.";
        break;
    }
    return mat;
}

QImage SSI_Image_Converter::mat_2_qimage(const cv::Mat& mat) {
    if (mat.type() == CV_8UC1 || mat.type() == CV_8U) {
        QImage image((const uchar *)mat.data, 
            mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        return image;
    }
    else if (mat.type() != CV_8UC3) 
        qDebug() << " mat_2_qimage err, mat.type(): " << mat.type();

    QImage image((const uchar *)mat.data, 
        mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    return image.rgbSwapped();  //r与b调换
}
