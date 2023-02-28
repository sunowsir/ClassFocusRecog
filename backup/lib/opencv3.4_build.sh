#! /bin/bash

# 安装python2支持
sudo apt-get install python-dev python-numpy
# 安装python3支持
sudo apt-get install python3-dev python3-numpy

# 安装gtk2支持
sudo apt-get install libgtk2.0-dev
# 安装gtk3支持
sudo apt-get install libgtk-3-dev

sudo apt-get install libpng-dev
sudo apt-get install libjpeg-dev
sudo apt-get install libopenexr-dev
sudo apt-get install libtiff-dev
sudo apt-get install libwebp-d

git clone -b 3.4 https://github.com/opencv/opencv.git

cd opencv
mkdir build
cd build

cmake ../ -DCMAKE_BUILD_TYPE=RELEASE -DBUILD_EXAMPLES=ON  -DBUILD_DOCS=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_TESTS=OFF

make -j4 
sudo make install 
