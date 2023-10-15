cmake_minimum_required(VERSION 3.10.1) 
project(infersdk)

option(USE_CUDA "use cuda" OFF) 
option(USE_OPENCV "use opencv" ON)

set(CMAKE_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/output)

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -fPIC -Wall -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-result")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS}")
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
endif()

# 设置输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIR}/bin)

set(ALL_LIBS "")
# 依赖库设置
include(cmake/depends.cmake)

message(STATUS "ALL_LIBS: ${ALL_LIBS}")

# 构建
#添加本项目头文件
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src) 



