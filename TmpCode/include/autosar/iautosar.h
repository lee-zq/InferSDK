#ifndef INTERFACE_H
#define INTERFACE_H
#include "message.h"

/*
    * 该接口类为C++接口类，用于封装C++接口
    * 该接口类为纯虚类，需要继承该类并实现接口
    * 接口类的实现类为Autosar类
*/
class IAutosar{
public:
    virtual int init(const std::string& cfg_path) = 0;
    virtual int uninit() = 0;
    virtual int start() = 0;
    virtual int stop() = 0;
    virtual int set_param() = 0;
    virtual int process(message* msg) = 0;

    virtual ~IAutosar(){};
};

#endif