# Student Status Identification
> 线上课堂状态监测系统

## 编译说明
1. 根据不同平台不同，这里不具体说明环境搭建；
2. 环境：c++、qt、opencv、dlib、make以及cmake；
3. 编译：

    1. `git clone https://github.com/sunowsir/Student_Status_Identification.git`
    2. `cd Student_Status_Identification`
    3. `mkdir build`
    4. `cd build`
    5. `cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ../`
    6. `make`
    7. 编译完成后会在`../bin`下生成可执行文件`SSI`

## 技术说明
1. 语言：`c++`；
2. 开发框架：`qt`；
3. 图像处理库：`opencv`、`dlib`；

## 设计说明
> 如下以图像处理为主，其他部分例如UI部分则简略介绍；

### UI

该程序使用qt来绘制UI，其中包括摄像头预览界面以及各种button等；
![ui](./Screenshot.png)

### 人脸识别器

1. 类声明如下

人脸识别器用于初步处理图像，并获取到人脸特征点，该类是表情识别的最基础的必须的类
```c++
/* 人脸识别器 */
class SSI_Face_Recognition {
private: 
    /* 人脸形状探测器 */
    dlib::shape_predictor sp;

    /* 一系列人脸所在区域 */
    std::vector<dlib::rectangle> faces;

    /* 人脸特征点分布 */
    std::vector<dlib::full_object_detection> shapes;

    /* 加载dlib的人脸识别器 */
    dlib::frontal_face_detector detector;

public: 
    SSI_Face_Recognition(const QString&);
    ~SSI_Face_Recognition();

    bool recognize(cv::Mat&);
    std::vector<dlib::rectangle>& faces_get();
    std::vector<dlib::full_object_detection>& shapes_get();
};
```

2. 对外接口详解

    1. `bool recognize(cv::Mat&)`

        使用人脸识别器时，先调用该方法，进行图像处理，在调用`faces_get`以及`shapes_get`获取数据;
        如下流程处理完毕后，数据会保存在类的`faces`以及`shapes`属性中；
        ```mermaid
        graph TB

        mstart(开始) --> cvtColor[ 提取灰度图像 ]
        cvtColor[ 提取灰度图像 ] --> assign[ 转化图片为矩阵数据 ]
        assign[ 转化图片为矩阵数据 ] --> detector[ 获取人脸区域 ]
        detector[ 获取人脸区域 ] --> get_sharps[ 获取人脸特征分布 ]
        get_sharps[ 获取人脸特征分布 ] --> mend(结束)
    ```

    1. `std::vector<dlib::rectangle>& faces_get()`
     
        返回类的`faces`属性，用于获取人脸区域数据；
    
    2. `std::vector<dlib::full_object_detection>& shapes_get()`
    
        返回类的`shapes`属性，用于获取特征值数据；


### 模型训练器

1. 类声明如下

```c++
/* 模型训练器 */
class SSI_Module_Trainer {
private: 

    /* 人脸形状探测器 */
    dlib::shape_predictor sp;

    /* dlib的matrix */
    dlib::array2d<dlib::bgr_pixel> dimg;

    /* 训练得到的系数数组 */
    float **trans_kp_arr;

    /* 表情标签数组 */
    int *face_label;

    /* 用于保存训练系数数组的行数 */
    unsigned int type_num;

    /* 用于保存训练系数数组的列数 */
    unsigned int img_num;

    /* 人脸识别器 */
    SSI_Face_Recognition *sfr;

private:
    /* 采集并保存特征点 */
    bool capture_and_save_keypoint(cv::Mat&, float* = nullptr);

public:

    SSI_Module_Trainer(int /* type num */, int /* img num */);
    ~SSI_Module_Trainer();

    /* 载入训练图片，以及该组图片对应的表情 */
    bool load_train_data(const QString& /* image path */,
        const int& /* 类别名称 */);

    /* 训练模型，并导出为xml模型文件 */
    bool train_module_2_xml();
};
```

2. 对外接口详解
 
    1. `bool load_train_data(const QString& /* image path */, const int& /* 类别名称 */);`
    
        该方法需传入两个参数，一个是训练图片的路径，另一个是这组图片是什么表情
        ```mermaid
        graph TB

        mstart(开始) --> args_parse[ 参数判断 ]
        args_parse[ 参数判断 ] --> read_img[ 读取图片 ]
        read_img[ 读取图片 ] --> csk[ 采集并保存特征点 ]
        csk[ 采集并保存特征点 ] --> mend(结束)
        ```
        1. 上述流程图中的 “**采集并保存特征点**”被封装到了一个内部方法`capture_and_save_keypoint`中：
        
            1. 方法原型：`bool capture_and_save_keypoint(cv::Mat&, float* = nullptr);`
            2. 流程如下：

                ```mermaid
                graph TB

                mstart(开始) --> face_reg[ 人脸识别 ]
                face_reg[ 人脸识别 ] --> calc_offset[ 计算系数 ]
                calc_offset[ 计算系数 ] --> calc_every_point[ 计算各点系数 ]
                calc_every_point[ 计算各点系数 ] --> save_offset[ 保存各点系数 ]
                save_offset[ 保存各点系数 ] --> mend(结束)
                ```

            3. 人脸识别：这里的人脸识别调用如上介绍的[人脸识别器](###人脸识别器)进行识别，得到面部区域以及特征点数据；
            4. 计算系数：流程中提到的 **计算系数** 指的是当前面部区域的高度；
            5. 计算各点系数：这里提到的 **计算各点系数** 指的是根据这两脸上的每一点计算得到的若干数据：
                1. 计算 $当前点到这张脸最左端距离 / 整张脸宽度$ 得到的数据保存下来;
                2. 计算 $当前点到这张脸最上端距离 / 整张脸高度$ 得到的数据保存下来;
            6. 保存各点系数：保存系数到类的属性里，当调用如下`bool train_module_2_xml();`方法时，将保存到属性中的系数传递给svm进行训练;

        2. 
    
    2. `bool train_module_2_xml();`
        
        创建svm对象，设置属性，传入各点计算得到的数据开始训练;

### 表情识别器

1. 类声明如下

```c++
/* 表情识别器 */
class SSI_Expression_Recognition {
private: 
    cv::Ptr<ns_CVML::SVM>   svm;

    /* 人脸识别器 */
    SSI_Face_Recognition    *sfr;

public:
    SSI_Expression_Recognition(const QString& /* module file path */);
    ~SSI_Expression_Recognition();

    /* 表情识别，传入图片，获得表情 */
    bool recognize(const QImage&, int& /* face type */);
};
```

2. 对外接口说明
    1. `bool recognize(const QImage&, int& /* face type */);`
        该方法用于表情识别，需传入待识别图片以及传出参数表情类型，识别结束后，通过第二个表情类型这个传出参数，将表情类型传出；
        ```mermaid
        graph TB
        
        mstart(开始) --> face_reg[ 人脸识别 ]
        face_reg[ 人脸识别 ] --> calc_offset[ 计算系数 ]
        calc_offset[ 计算系数 ] --> calc_every_point[ 计算各点系数 ]
        calc_every_point[ 计算各点系数 ] --> query[ 调用模型查询 ]
        query[ 调用模型查询 ] --> ret[ 传出结果 ]
        ret[ 传出结果 ] --> mend(结束)

        ```

---
参考：
1. https://blog.csdn.net/u012819339/article/details/82262915
2. https://blog.csdn.net/zmdsjtu/article/details/53667929
