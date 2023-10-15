

file(GLOB_RECURSE SRC_LIST *.cpp)  # 递归搜索匹配
message(STATUS "SOLUTION SRC_LIST: ${SRC_LIST}")
add_library(${CVServerLibName} SHARED ${SRC_LIST})
target_link_libraries(${CVServerLibName} ${OpenCV_LIBRARIES})
target_link_libraries(${CVServerLibName} ${ORT_LIBS})