cmake_minimum_required(VERSION 3.5)

project(OCSFS VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 可执行文件输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/ocsfs_mgr_cli)

set(TS_FILES OCSFS_zh_CN.ts)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    dlib::dlib
)

add_executable(ocsfs_mgr_cli
    src/main.cpp
    src/ocsfs_window.cpp
    src/ocsfs_connect_widget.cpp
    src/ocsfs_login_widget.cpp
    src/ocsfs_widget.cpp
    src/ocsfs_client.cpp
    src/ocsfs_info_dialog.cpp
    src/ocsfs_pic_show_dialog.cpp
    src/ocsfs_interactive_dialog.cpp
    src/ocsfs_student_info_dialog.cpp
    src/ocsfs_barchar_label.cpp
    src/ocsfs_float_widget.cpp
    # src/ocsfs_processthree_widget.cpp
    ${TS_FILES}
)

target_link_libraries(ocsfs_mgr_cli
    PRIVATE Qt6::Widgets Qt6::Multimedia Qt6::MultimediaWidgets Qt6::Charts
    ${OpenCV_LIBS}
    dlib::dlib
)

