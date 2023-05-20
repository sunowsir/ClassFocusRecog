cmake_minimum_required(VERSION 3.5)

project(OCSFS VERSION 0.1 LANGUAGES CXX)

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

include_directories(
    ${CMAKE_SOURCE_DIR}/src/include
)

add_subdirectory(src/ocsfs_serv)
add_subdirectory(src/ocsfs_mgr_cli)
# add_subdirectory(src/ocsfs_user_cli)
# add_subdirectory(src/module_train_tool)
# add_subdirectory(src/demo)

