#include "message.h"
#include "interface.h"

class Android : public IAndroid{
public:
    Android()=default;
    ~Android()=default;

    virtual int Send(message* msg);

    virtual int init(IAutosar* autosar_ptr){
        autosar_ = autosar_ptr;
        return 0;
    }
public:
    IAutosar* autosar_ = nullptr;
};

// 封装C接口
extern "C"{
    int createAndroid(void** inst){
        auto p = new Android;
        *inst = p;
        return 0;
    }
    int destroyAndroid(void* android){
        IAndroid *p = (IAndroid*)android;
        delete p;
        return 0;
    }
    int initAndroid(void* android, void* autosar){
        IAndroid *p = (IAndroid*)android;
        IAutosar *q = (IAutosar*)autosar;
        return p->init(q);
    }
    int processAndroid(void* android, message* msg){
        IAndroid *p = (IAndroid*)android;
        return p->Send(msg);
    }
}