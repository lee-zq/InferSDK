#pragma once

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <vector>

// 宏，用于打印未实现的日志
#define NOT_IMPLEMENTED()                                                      \
    do                                                                         \
    {                                                                          \
        std::cout << "Not implemented: " << __FUNCTION__ << " | " << __FILE__  \
                  << ":" << __LINE__ << std::endl;                             \
    } while (0)
