# 设置工程名称
project(autosar) 
cmake_minimum_required(VERSION 2.8)
# 设置可执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
# 设置库文件输出目录
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -O0")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -O0")
# 设置头文件包含路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 引入OpenCV库
# 设置opencv路径
set(OpenCV_DIR /opt/envs/opencvlib/share/OpenCV)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
link_libraries(${OpenCV_LIBRARIES})

# 遍历文件
file(GLOB_RECURSE src_all_files
src/*.cpp
)
add_library(autosar SHARED ${src_all_files})

file(GLOB_RECURSE test_all_files
test/*.cpp test/*.c
)
add_executable(autosar_test ${test_all_files})
target_link_libraries(autosar_test autosar ${OpenCV_LIBS})
