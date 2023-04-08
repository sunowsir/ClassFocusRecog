cmake_minimum_required(VERSION 3.5)

project(CFR VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 可执行文件输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/cfr_mgr_cli)

set(TS_FILES CFR_zh_CN.ts)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    dlib::dlib
)

add_executable(cfr_mgr_cli
    src/main.cpp
    src/cfr_window.cpp
    src/cfr_widget.cpp
    src/cfr_module_trainer.cpp
    src/cfr_face_recognition.cpp
    src/cfr_image_converter.cpp
    src/cfr_expression_recognition.cpp
    ${TS_FILES}
)

target_link_libraries(cfr_mgr_cli
    PRIVATE Qt6::Widgets Qt6::Multimedia Qt6::MultimediaWidgets
    ${OpenCV_LIBS}
    dlib::dlib
)

