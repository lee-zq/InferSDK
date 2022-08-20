
file(GLOB_RECURSE SRC_LIST "*.cpp")             # 递归搜索匹配
add_library(infersdk SHARED ${SRC_LIST})
target_link_libraries(infersdk ${OpenCV_LIBRARIES})