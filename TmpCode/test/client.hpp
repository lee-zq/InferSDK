#include "message.h"
#include "interface.h"

class Client{
public:
    int Send(message* msg){
        autosar_->process(msg);
        return 0;
    }
    // 初始化msg
    int init_msg(){
        msg.id = 1;
        msg.data = 2;
        return 0;
    }

    virtual int init(IAutosar* autosar_ptr){
        autosar_ = autosar_ptr;
        return 0;
    }
public:
    IAutosar* autosar_ = nullptr;
    message msg;
};