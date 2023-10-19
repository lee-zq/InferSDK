#pragma once
#include "error_code.h"
#include "cv_server/message.h"
#include "common/define.hpp"
#include <thread>
#include "arch/inst_manager.h"
#include <queue>
#include <mutex>
#include <condition_variable>

#include "common/threadpool/threadpool.hpp"

class CVServer{
public:
    virtual int init(std::string cfg_path);
    virtual int fini();
    // virtual int start();
    // virtual int stop();
    int get_inst(FID fid, Instance** inst_ptr);
    virtual int process(message* msg);
private:
    std::map<FID, Instance*> inst_map_;
    std::mutex inst_map_mutex_;
    // 执行线程池
    std::shared_ptr<ThreadPool> thread_pool_;
};