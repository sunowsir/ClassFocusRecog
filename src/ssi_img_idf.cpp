/*
	* File     : ssi_img_idf.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月07日 星期二 19时44分57秒
*/

#include "ssi_img_idf.h"

cv::Mat SSI_Img_Idf::qimage_2_mat(const QImage& image) {
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
    }
    return mat;
}

QImage SSI_Img_Idf::mat_2_qimage(const cv::Mat& mat) {
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

bool SSI_Img_Idf::idf_core(cv::Mat& frame) {
    cv::Mat dst;
    
    //提取灰度图
    cv::cvtColor(frame, dst, CV_BGR2GRAY);

    //加载dlib的人脸识别器
    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();

    //加载人脸形状探测器
    dlib::shape_predictor sp;
    dlib::deserialize(QCoreApplication::applicationDirPath().toStdString() + 
        "/shape_predictor_68_face_landmarks.dat") >> sp;

    //Mat转化为dlib的matrix
    dlib::array2d<dlib::bgr_pixel> dimg;
    dlib::assign_image(dimg, dlib::cv_image<uchar>(dst)); 

    //获取一系列人脸所在区域
    std::vector<dlib::rectangle> dets = detector(dimg);
    std::cout << "Number of faces detected: " << dets.size() << std::endl;

    if (dets.size() == 0)
        return false;

    //获取人脸特征点分布
    std::vector<dlib::full_object_detection> shapes;
    int i = 0;
    for (i = 0; i < dets.size(); i++) {
        dlib::full_object_detection shape = sp(dimg, dets[i]); //获取指定一个区域的人脸形状
        shapes.push_back(shape); 
    }   

    //指出每个检测到的人脸的位置
    for (i=0; i<dets.size(); i++) {
        //画出人脸所在区域
        cv::Rect r;
        r.x = dets[i].left();
        r.y = dets[i].top();
        r.width = dets[i].width();
        r.height = dets[i].height();
        cv::rectangle(frame, r, cv::Scalar(0, 0, 255), 1, 1, 0); 
    }

    this->line_one_face_detections(frame, shapes);

    return true;
}

void SSI_Img_Idf::line_one_face_detections (cv::Mat img, std::vector<dlib::full_object_detection> fs) {
    int i, j;
    for (j=0; j<fs.size(); j++) {
        cv::Point p1, p2;
        for (i = 0; i<67; i++) {
            // 下巴到脸颊 0 ~ 16
            //左边眉毛 17 ~ 21
            //右边眉毛 21 ~ 26
            //鼻梁     27 ~ 30
            //鼻孔        31 ~ 35
            //左眼        36 ~ 41
            //右眼        42 ~ 47
            //嘴唇外圈  48 ~ 59
            //嘴唇内圈  59 ~ 67
            switch (i) {
                case 16:
                case 21:
                case 26:
                case 30:
                case 35:
                case 41:
                case 47:
                case 59:
                    i++;
                    break;
                default:
                    break;
            }

            p1.x = fs[j].part(i).x();
            p1.y = fs[j].part(i).y();
            p2.x = fs[j].part(i+1).x();
            p2.y = fs[j].part(i+1).y();
            cv::line(img, p1, p2, cv::Scalar(0,0,255), 2, 4, 0);
        }
    }
}

SSI_Img_Idf::SSI_Img_Idf() {
    
}

SSI_Img_Idf::~SSI_Img_Idf() {
    
}

QImage SSI_Img_Idf::image_identification(const QImage& img) {
    cv::Mat frame = this->qimage_2_mat(img);

    QImage ret;
    if (this->idf_core(frame) == false) {
        qDebug() << "image_identification false";
        return ret;
    }

    ret = this->mat_2_qimage(frame);
    
    return ret;
}
