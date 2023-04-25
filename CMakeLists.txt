cmake_minimum_required(VERSION 3.5)

project(SSI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt6 COMPONENTS Multimedia MultimediaWidgets REQUIRED)

# OpenCV
set(OpenCV_DIR /usr/local/lib)
find_package(OpenCV REQUIRED)

# dlib
find_package(dlib REQUIRED)

add_subdirectory(src/cfr_serv)
add_subdirectory(src/cfr_mgr_cli)
add_subdirectory(src/cfr_user_cli)
add_subdirectory(src/module_train_tool)

