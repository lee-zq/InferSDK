
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

include_directories(${SOURCE_DIR}/common)
include_directories(${SOURCE_DIR}/module)

file(GLOB_RECURSE SRC_LIST "*.cc")             # 递归搜索匹配
add_library(cvserver SHARED ${SRC_LIST})
target_link_libraries(cvserver ${OpenCV_LIBRARIES})
target_link_libraries(cvserver ${ORT_LIBS})