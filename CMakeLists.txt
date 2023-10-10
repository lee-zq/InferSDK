cmake_minimum_required(VERSION 3.10.1) 
project(infersdk)
option(USE_CUDA "<help_text>" OFF) 

set(ALL_LIBS "")

# 添加opencv依赖
set(OpenCV_DIR /opt/envs/opencvlib/share/OpenCV)
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV include dir:${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libs: ${OpenCV_LIBS}")
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
list(APPEND ALL_LIBS ${OpenCV_LIBS})

# 添加CUDA依赖
if(USE_CUDA)
    # set(CUDA_ROOT /usr/local/cuda)
    find_package(CUDA REQUIRED)
    message(STATUS "CUDA include dir: ${CUDA_INCLUDE_DIRS}")
    message(STATUS "CUDA libs: ${CUDA_LIBRARIES}")
    INCLUDE_DIRECTORIES(${CUDA_INCLUDE_DIRS})
    list(APPEND ALL_LIBS ${CUDA_LIBRARIES})
endif(USE_CUDA)

# 添加推理库依赖
#onnxruntime
set(ORT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include/onnxruntime)
set(ORT_LIBS_DIR "/root/projects/InferSDK/lib/onnxruntime")
set(ORT_LIBS ${ORT_LIBS_DIR}/libonnxruntime.so)

INCLUDE_DIRECTORIES(${ORT_INCLUDE_DIRS})
list(APPEND ALL_LIBS ${ORT_LIBS})

# 构建
#添加本项目头文件
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/infersdk)

add_subdirectory(src) 
add_subdirectory(test) 



