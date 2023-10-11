#include "message.h"
#include "icv_server.h"
#include "../common/define.hpp"

class CVServer{
public:
    virtual int init(const std::string& cfg_path) override;
    virtual int uninit() override;
    virtual int start() override;
    virtual int stop() override;
    virtual int set_param() override{
        NOT_IMPLEMENTED();
        return 0;
    }
    virtual int process(message* msg) override{
        NOT_IMPLEMENTED();
        return 0;
    }
};