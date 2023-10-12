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

    int submit(std::string& file_path){
        Send(&msg);
        return 0;
    }

    virtual int init(void* cv_server_ptr){
        cv_server_ = cv_server_ptr;
        return 0;
    }
public:
    void* cv_server_ = nullptr;
    message msg;
};

