#include "message.h"
#include "interface.h"

class Client{
public:
    int Send(message* msg){
        cv_server_->process(msg);
        return 0;
    }
    // 初始化msg
    int init_msg(){
        msg.id = 1;
        msg.data = 2;
        return 0;
    }

    virtual int init(ICVServer* cv_server_ptr){
        cv_server_ = cv_server_ptr;
        return 0;
    }
public:
    ICVServer* cv_server_ = nullptr;
    message msg;
};