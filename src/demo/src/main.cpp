/*
	* File     : demo.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月04日 星期四 16时24分45秒
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <dlib/opencv.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>

using namespace std;
using namespace cv;
using namespace dlib;

int main()
{
    // 加载人脸检测器
    frontal_face_detector detector = get_frontal_face_detector();

    // 加载面部标志检测器
    shape_predictor predictor;
    deserialize("shape_predictor_68_face_landmarks.dat") >> predictor;


    // 读取一帧图像
    Mat frame = imread("1.jpg");

    // 将OpenCV图像转换为Dlib图像
    cv_image<bgr_pixel> dlib_img(frame);

    // 检测面部
    std::vector<dlib::rectangle> faces = detector(dlib_img);

    // 对于每个检测到的面部
    for (auto&& face : faces) {
        // 检测面部标志
        full_object_detection landmarks = predictor(dlib_img, face);

        // 计算面部姿势
        double yaw = landmarks.part(45).x() - landmarks.part(36).x();
        double pitch = landmarks.part(8).y() - (landmarks.part(27).y() + landmarks.part(28).y()) / 2;

        // 判断是否低头或侧脸
        if (yaw < -20 || yaw > 20 || pitch > 10) {
            dlib::rectangle rect = face;

            dlib::rectangle rect_scaled(rect.left() * 2, rect.top() * 2, rect.right() * 2, rect.bottom() * 2);

            // dlib::rectangle(frame, rect_scaled, Scalar(0, 0, 255), 2);
            std::cout << "1" << std::endl;
        }
        else {
            dlib::rectangle rect = face;
            dlib::rectangle rect_scaled(rect.left() * 2, rect.top() * 2, rect.right() * 2, rect.bottom() * 2);
            // cv::rectangle(frame, rect_scaled, Scalar(0, 255, 0), 2);
            std::cout << "2" << std::endl;
        }
    }

    // 显示结果
    imshow("Face Detection", frame);


    return 0;
}

