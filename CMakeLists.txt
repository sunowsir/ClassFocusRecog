# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (demo)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(.. DIR_SRCS)

# OpenCV
# 当有多个opencv版本时，可指定优先查找opencv的目录OpenCV_DIR
set(OpenCV_DIR /usr/local/lib)
find_package(OpenCV REQUIRED)
find_package(dlib REQUIRED)

# 可执行文件输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# 添加include文件查找目录，这里把CMakeLists.txt所在目录添加了进去
include_directories(
    ${PROJECT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    dlib::dlib
    src
)
# 指定生成目标 
add_executable(demo
    demo.cpp
)

# 添加链接库
target_link_libraries(demo
    ${OpenCV_LIBS}
    dlib::dlib
)
