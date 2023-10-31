
add_executable(test_cvserver test_cxx_api.cpp)
target_link_libraries(test_cvserver ${CVServerLibName} ${OpenCV_LIBS})
