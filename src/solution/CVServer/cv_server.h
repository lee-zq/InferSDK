#include "message.h"
#include "icv_server.h"
#include "../common/define.hpp"

class CVServer{
public:
    virtual int init(const std::string& cfg_path);
    virtual int fini();
    virtual int start();
    virtual int stop();
    virtual int set_param(){
        NOT_IMPLEMENTED();
        return 0;
    }
    virtual int process(message* msg){
        NOT_IMPLEMENTED();
        return 0;
    }
};