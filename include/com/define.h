#pragma once

#include <algorithm>
#include <cstdio>
#include <iostream>

#define NAMESPACE_BEGIN(name)                                                  \
    namespace name                                                             \
    {
#define NAMESPACE_END(name) }

#define SPACE_NAME iSDK

#define SPACE_BEGIN NAMESPACE_BEGIN(SPACE_NAME)
#define SPACE_END NAMESPACE_END(SPACE_NAME)

// 宏，用于打印未实现的日志
#define NOT_IMPLEMENTED()                                                      \
    do                                                                         \
    {                                                                          \
        std::cout << "Not implemented: " << __FUNCTION__ << " | " << __FILE__  \
                  << ":" << __LINE__ << std::endl;                             \
    } while (0)