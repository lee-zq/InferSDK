
file(GLOB_RECURSE SRC_LIST *.cc *.cpp *.hpp)  # 递归搜索匹配
message(STATUS "SOLUTION SRC_LIST: ${SRC_LIST}")
add_library(cvserver SHARED ${SRC_LIST})
target_link_libraries(cvserver ${OpenCV_LIBRARIES})
target_link_libraries(cvserver ${ORT_LIBS})