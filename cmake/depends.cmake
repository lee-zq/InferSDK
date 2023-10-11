
# 添加opencv依赖
if(USE_OPENCV)
    set(OpenCV_DIR $ENV{OpenCV_DIR})
    find_package(OpenCV REQUIRED)
    message(STATUS "OpenCV include dir:${OpenCV_INCLUDE_DIRS}")
    message(STATUS "OpenCV libs: ${OpenCV_LIBS}")
    INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
    list(APPEND ALL_LIBS ${OpenCV_LIBS})
endif()

# 添加CUDA依赖
if(USE_CUDA)
    set(CUDA_ROOT $ENV{CUDA_ROOT})
    find_package(CUDA REQUIRED)
    message(STATUS "CUDA include dir: ${CUDA_INCLUDE_DIRS}")
    message(STATUS "CUDA libs: ${CUDA_LIBRARIES}")
    INCLUDE_DIRECTORIES(${CUDA_INCLUDE_DIRS})
    list(APPEND ALL_LIBS ${CUDA_LIBRARIES})
endif(USE_CUDA)

# 添加推理引擎库依赖
# onnxruntime
set(ORT_ROOT ${CMAKE_SOURCE_DIR}/thirdparty/infer_engine/onnxruntime)
set(ORT_INCLUDE_DIRS ${ORT_ROOT}/include)
set(ORT_LIBS_DIR ${ORT_ROOT}/lib64)
set(ORT_LIBS ${ORT_LIBS_DIR}/libonnxruntime.so)
INCLUDE_DIRECTORIES(${ORT_INCLUDE_DIRS})
list(APPEND ALL_LIBS ${ORT_LIBS})

# 添加spdlog库依赖
set(SPDLOG_ROOT ${CMAKE_SOURCE_DIR}/thirdparty/spdlog)
set(SPDLOG_INCLUDE_DIRS ${SPDLOG_ROOT}/include)
INCLUDE_DIRECTORIES(${SPDLOG_INCLUDE_DIRS})

# 添加json库依赖
set(JSON_ROOT ${CMAKE_SOURCE_DIR}/thirdparty/json)
set(JSON_INCLUDE_DIRS ${JSON_ROOT}/include)
INCLUDE_DIRECTORIES(${JSON_INCLUDE_DIRS})
