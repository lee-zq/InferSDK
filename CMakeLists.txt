cmake_minimum_required(VERSION 3.10.1) 
project(infersdk)

option(USE_CUDA "<help_text>" OFF) 
option(USE_OPENCV "<help_text>" ON)

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c++11")
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-result")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-result")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-result")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-result")
endif()

# 设置输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(ALL_LIBS "")
# 依赖库设置
include(cmake/depends.cmake)

# 构建
#添加本项目头文件
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src) 



