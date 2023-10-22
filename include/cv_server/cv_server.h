#pragma once
#include "arch/inst_manager.h"
#include "com/utils/define.hpp"
#include "cv_server/message.h"
#include "error_code.h"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#include "com/threadpool/threadpool.hpp"

class CVServer
{
public:
    int init(std::string cfg_path);
    int fini();
    // int start();
    // int stop();
    int get_inst(TaskType task_type, Instance** inst_ptr);
    int process(message* msg);

private:
    bool initialized_ = false;
    std::map<TaskType, Instance*> inst_map_;
    std::mutex inst_map_mutex_;
    // 执行线程池
    std::shared_ptr<ThreadPool> thread_pool_;
};