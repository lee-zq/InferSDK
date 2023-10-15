
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

include_directories(${SOURCE_DIR})
include_directories(${SOURCE_DIR}/common)
include_directories(${SOURCE_DIR}/module)

file(GLOB_RECURSE ARCH_FILE arch/*.cpp)
file(GLOB_RECURSE ENGINE_FILE engine/*.cpp)
file(GLOB_RECURSE MODULE_FILE module/*.cpp)
file(GLOB_RECURSE COMMON_FILE common/*.cpp)
# 构建核心库
add_library(core ${ARCH_FILE} ${ENGINE_FILE} ${MODULE_FILE} ${COMMON_FILE})

# 构建接口库
set(CVServerLibName cvserver)
add_subdirectory(solution/CVServer)

# 构建测试程序
add_subdirectory(solution/test)