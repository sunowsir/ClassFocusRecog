/*
	* File     : ssi_module_trainer.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年03月07日 星期二 19时44分57秒
*/

#include "ssi_module_trainer.h"

cv::Mat SSI_Module_Trainer::qimage_2_mat(const QImage& image) {
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

QImage SSI_Module_Trainer::mat_2_qimage(const cv::Mat& mat) {
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

bool SSI_Module_Trainer::idf_core(cv::Mat &frame) {
    cv::Mat dst;
    
    //提取灰度图
    cv::cvtColor(frame, dst, CV_BGR2GRAY);

    //加载dlib的人脸识别器
    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();

    //加载人脸形状探测器
    dlib::deserialize(QCoreApplication::applicationDirPath().toStdString() + 
        "/shape_predictor_68_face_landmarks.dat") >> this->sp;

    //Mat转化为dlib的matrix
    dlib::assign_image(this->dimg, dlib::cv_image<uchar>(dst)); 

    //获取一系列人脸所在区域
    this->dets = detector(dimg);
    std::cout << "Number of faces detected: " << dets.size() << std::endl;

    if (this->dets.size() == 0)
        return false;

    //获取人脸特征点分布
    for (int i = 0; i < this->dets.size(); i++) {
        dlib::full_object_detection shape = sp(dimg, dets[i]); //获取指定一个区域的人脸形状
        this->shapes.push_back(shape); 
    }   
    if (this->shapes.empty()) {
        qDebug() << "shapes is empty";
        return false;
    }

    return true;
}

bool SSI_Module_Trainer::point_face_detections (cv::Mat &frame) {
    //指出每个检测到的人脸的位置
    for (int i = 0; i < this->dets.size(); i++) {
        //画出人脸所在区域
        cv::Rect r;
        r.x = this->dets[i].left();
        r.y = this->dets[i].top();
        r.width = this->dets[i].width();
        r.height = this->dets[i].height();
        cv::rectangle(frame, r, cv::Scalar(0, 0, 255), 1, 1, 0); 
    }

    for (int j = 0; j < this->shapes.size(); j++) {
        for (int i = 0; i < 67; i++) {

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

            cv::Point p1, p2;

            p1.x = this->shapes[j].part(i).x();
            p1.y = this->shapes[j].part(i).y();

            cv::circle(frame, p1, 2, cv::Scalar(0, 0, 255), -1);


            // p1.x = fs[j].part(i).x();
            // p1.y = fs[j].part(i).y();
            // p2.x = fs[j].part(i+1).x();
            // p2.y = fs[j].part(i+1).y();
            // cv::line(img, p1, p2, cv::Scalar(0,0,255), 2, 4, 0);
        }
    }

    return true;
}

bool SSI_Module_Trainer::capture_and_save_keypoint(float *kp_offset_arr) {
    /* 系数 */
    float offset = -(this->dets[0].top() - this->dets[0].bottom());

    int kp_offset_arr_num = 0;
    for (int j = 0; j < 68; j++) {

        float kp_offset = this->shapes[0].part(j).x() - this->dets[0].left()  / offset;
        this->kp_offsets->push_back(kp_offset);
        if (nullptr != kp_offset_arr)
            kp_offset_arr[kp_offset_arr_num++] = kp_offset;

        kp_offset = this->shapes[0].part(j).y() - this->dets[0].left()  / offset;
        this->kp_offsets->push_back(kp_offset);
        if (nullptr != kp_offset_arr)
            kp_offset_arr[kp_offset_arr_num++] = kp_offset;
    }

    return true;
}

SSI_Module_Trainer::SSI_Module_Trainer(int tnum, int inum) {
    this->kp_offsets = new std::vector<float>();

    if (tnum <= 0) {
        qDebug() << "type_num <= 0";
        return ;
    }
    if (inum <= 0) {
        qDebug() << "img_num <= 0";
        return ;
    }

    this->type_num = tnum;
    this->img_num = inum;

    int row = (this->type_num * this->img_num);

    this->trans_kp_arr = (float **)malloc(sizeof(float*) * row);
    for (int i = 0; i < row; i++) {
        this->trans_kp_arr[i] = (float*)malloc(sizeof(float) * (68 * 2));
        memset(this->trans_kp_arr[i], 0, sizeof(float) * (68 * 2));
    }

    this->face_label = (int*)malloc(sizeof(int) * row);
    memset(this->face_label, 0, sizeof(int) * row);

    return ;
}

SSI_Module_Trainer::~SSI_Module_Trainer() {
    delete this->kp_offsets;

    int row = (this->type_num * this->img_num);
    for (int i = 0; i < row; i++) {
        free(this->trans_kp_arr[i]);
        this->trans_kp_arr[i] = nullptr;
    }
    free(this->trans_kp_arr);
    this->trans_kp_arr = nullptr;

    free(this->face_label);

    return ;
}

QImage SSI_Module_Trainer::image_identification(const QImage& img) {
    cv::Mat frame = this->qimage_2_mat(img);

    QImage ret;
    if (this->idf_core(frame) == false) {
        qDebug() << "image_identification false";
        return ret;
    }

    this->point_face_detections(frame);
    this->capture_and_save_keypoint();

    ret = this->mat_2_qimage(frame);
    
    return ret;
}

bool SSI_Module_Trainer::load_train_data(const QString& img_path, 
    const int& face_type) {
    if (face_type < SSI_FACE_BASE) {
        qDebug() << "face_type < SSI_FACE_BASE";
        return false;
    }
    if (QString("") == img_path) {
        qDebug() << "img_path is null";
        return false;
    }

    int row = (this->type_num * this->img_num);

    for (int i = 0; i < this->img_num; i++) {
        this->face_label[i + ((face_type - SSI_FACE_BASE) * this->img_num)] = face_type;
    }

    int img_idx = (face_type - SSI_FACE_BASE) * this->img_num;
    /* 遍历目录中的所有文件 (code from ChatGPT) */
    for (const auto & entry : ns_fs::directory_iterator(img_path.toStdString())) {
        if (img_idx - (face_type - SSI_FACE_BASE) * this->img_num >= this->img_num)
            break;

        /* 检查文件是否为图像 */
        if (entry.path().extension() == ".jpg" || 
            entry.path().extension() == ".png") {

            std::cout << entry.path().string() << std::endl;

            /* 将图像文件读入 Mat 对象 */
            cv::Mat frame = cv::imread(entry.path().string());

            /* 在此处对图像进行处理 */
            if (false == this->idf_core(frame)) {
                qDebug() << "image_identification false";
                continue;
            }

            this->capture_and_save_keypoint(this->trans_kp_arr[img_idx++]);
        }
    }
    
    return true;
}

bool SSI_Module_Trainer::train_module_2_xml() {
    int row = (this->type_num * this->img_num);

    cv::Mat train_mat(row, (2 * 68), CV_32FC1, this->trans_kp_arr);
    cv::Mat label_mat(row, 1, CV_32SC1, this->face_label);

    cv::Ptr<ns_CVML::SVM> svm = ns_CVML::SVM::create();
    svm->setType(ns_CVML::SVM::C_SVC);
    svm->setKernel(ns_CVML::SVM::LINEAR);
    svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1E-6));

    svm->train(train_mat, ns_CVML::ROW_SAMPLE, label_mat);

    std::string xml_fname = std::string(QCoreApplication::applicationDirPath().toStdString() + 
        std::string("/SVM_DATA.xml"));
    svm->save(xml_fname);

    qDebug() << "over!!!";

    return true;
}
