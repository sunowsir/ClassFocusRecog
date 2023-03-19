# Student Status Identification
> 线上课堂状态监测系统

## 编译说明
1. 根据不同平台不同，这里具体说明环境搭建；
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

# 设计说明

---
参考：
1. https://blog.csdn.net/u012819339/article/details/82262915
2. https://blog.csdn.net/zmdsjtu/article/details/53667929
