
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

include_directories(${SOURCE_DIR})
include_directories(${SOURCE_DIR}/common)
include_directories(${SOURCE_DIR}/module)

add_subdirectory(solution/CVServer)
# add_subdirectory(solution/test)
# add_subdirectory(solution/CVServer)