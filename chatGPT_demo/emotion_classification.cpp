/*
	* File     : emotion_classification.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月11日 星期六 01时18分18秒
*/

#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <dlib/svm_threaded.h>
#include <dlib/data_io.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>

using namespace std;
using namespace cv;
using namespace dlib;

// 定义表情分类器
typedef matrix<float, 128, 1> sample_type;
typedef linear_kernel<sample_type> kernel_type;
typedef svm_c_trainer<kernel_type> trainer_type;
typedef decision_function<kernel_type> function_type;
function_type trained_svm;

// 定义人脸检测器和关键点检测器
frontal_face_detector face_detector;
shape_predictor shape_predictor_model;

// 加载表情分类器
void load_svm(string model_file) {
    deserialize(model_file) >> trained_svm;
}

// 提取面部特征
sample_type extract_features(Mat& face_roi) {
    cv_image<bgr_pixel> img(face_roi);
    shape_predictor_model(img, face);
    sample_type sample;
    for (int i = 0; i < 128; ++i) {
        sample(i) = face_descriptor[i];
    }
    return sample;
}

// 表情识别
string recognize_expression(Mat& frame) {
    // 检测面部
    cv_image<bgr_pixel> img(frame);
    std::vector<rectangle> faces = face_detector(img);
    if (faces.size() == 0) {
        return "Unknown";
    }
    full_object_detection shape = shape_predictor_model(img, faces[0]);

    // 提取面部特征
    Mat face_roi = frame(Rect(faces[0].left(), faces[0].top(), faces[0].width(), faces[0].height()));
    sample_type sample = extract_features(face_roi);

    // 进行表情分类
    float score = trained_svm(sample);
    if (score > 0) {
        return "Smile";
    } else {
        return "Neutral";
    }
}

int main(int argc, char** argv) {
    // 加载人脸检测器和关键点检测器
    face_detector = get_frontal_face_detector();
    deserialize("shape_predictor_68_face_landmarks.dat") >> shape_predictor_model;

    // 加载表情分类器
    load_svm("emotion_classification.svm");

    // 打开摄像头
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "Failed to open camera" << endl;
        return -1;
    }

    // 实时表情识别
    while (true) {
        Mat frame;
        cap >> frame;
        string expression = recognize_expression(frame);
        putText(frame, expression, Point(20, 40), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
        imshow("Emotion Recognition", frame);
        if (waitKey(30) == 'q') {
            break;
        }
    }

    return 0;
}

