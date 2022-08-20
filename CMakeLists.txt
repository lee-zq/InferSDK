cmake_minimum_required(VERSION3.20.1) 
project(infersdk)
option(USE_CUDA "<help_text>" OFF) 

set(ALL_LIBS "")


# 添加opencv依赖
set(OpenCV_DIR /usr/local/opencv34)
find_package(OpenCV REQUIRED)
if(not OPENCV_FOUND)
    message(FATAL "opencv not found!")
endif(OPENCV_FOUND)

INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
list(APPEND ALL_LIBS ${OpenCV_LIBS})
# 构建
add_subdirectory(src) 
add_subdirectory(test) 



