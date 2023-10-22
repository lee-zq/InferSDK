#pragma once
#include "arch/inst_manager.h"
#include "common/define.hpp"
#include "cv_server/message.h"
#include "error_code.h"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#include "common/threadpool/threadpool.hpp"

class CVServer
{
public:
    virtual int init(std::string cfg_path);
    virtual int fini();
    // virtual int start();
    // virtual int stop();
    int get_inst(TaskType task_type, Instance** inst_ptr);
    virtual int process(message* msg);

private:
    std::map<TaskType, Instance*> inst_map_;
    std::mutex inst_map_mutex_;
    // 执行线程池
    std::shared_ptr<ThreadPool> thread_pool_;
};