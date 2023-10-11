#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <mutex>

// 宏，用于打印未实现的日志
#define NOT_IMPLEMENTED() \
    do{ \
        std::cout << "Not implemented: " << __FUNCTION__ << \
        " | "<< __FILE__ << ":" << __LINE__ << std::endl; \
    }while(0)