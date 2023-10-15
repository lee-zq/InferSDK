
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

include_directories(${SOURCE_DIR})
include_directories(${SOURCE_DIR}/common)
include_directories(${SOURCE_DIR}/module)

file(GLOB_RECURSE ARCH_FILE   arch/*.cpp)
file(GLOB_RECURSE ENGINE_FILE engine/*.cpp)
file(GLOB_RECURSE MODULE_FILE module/*.cpp)
file(GLOB_RECURSE COMMON_FILE common/*.cpp)
# 构建核心库
set(CoreLibName core)
set(CORE_SRC ${ARCH_FILE} ${ENGINE_FILE} ${MODULE_FILE} ${COMMON_FILE})
message(STATUS "CORE SRC: ${CORE_SRC}")
# add_library(${CoreLibName} SHARED ${CORE_SRC})
add_library(${CoreLibName} STATIC ${CORE_SRC})
target_link_libraries(${CoreLibName} ${ALL_LIBS})

# 构建接口库
set(CVServerLibName cvserver)
add_subdirectory(solution/CVServer)

# 构建测试程序
add_subdirectory(solution/test)