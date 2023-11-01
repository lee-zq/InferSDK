#pragma once
#include "interface/base_instance.h"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>

#include "../instance/common_inst.h"
#include "all_type.h"
#include "com/logger.h"
#include "cv_server/error_code.h"
#include "cv_server/message.h"

class InstPool
{
public:
    InstPool()
    {
        initialized_ = false;
    }
    ~InstPool()
    {
        if (initialized_)
        {
            fini();
        }
    }

    int init(const std::string& init_param)
    {
        busy_inst_map_.clear();
        idle_inst_map_.clear();
        initialized_ = true;
        LInfo("InstPool initialize success...");
        return 0;
    }

    int fini()
    {
        if (busy_inst_map_.size() > 0)
        {
            LError("InstPool fini failed, busy_inst_map_ size={}", busy_inst_map_.size());
            return ERR_INVALID_STATE;
        }
        for (auto& inst_ptr : idle_inst_map_)
        {
            int ret = inst_ptr->fini();
            log_error_return(ret, "InstPool fini failed, inst fini failed. ret={}", ret);
            delete inst_ptr;
        }
        idle_inst_map_.clear();
        for (auto& inst_ptr : busy_inst_map_)
        {
            int ret = inst_ptr->fini();
            log_error_return(ret, "InstPool fini failed, inst fini failed. ret={}", ret);
            delete inst_ptr;
        }
        busy_inst_map_.clear();
        initialized_ = false;
        LInfo("InstPool finalize success.");
        return 0;
    }

    int add_inst(InstanceBase* inst_ptr)
    {
        if (inst_ptr == nullptr)
        {
            LError("InstPool add inst failed, inst_ptr is nullptr");
            return ERR_INVALID_PARAM;
        }
        std::lock_guard<std::mutex> lock(inst_pool_mutex_);
        idle_inst_map_.insert(inst_ptr);
        return 0;
    }

    int delete_inst(InstanceBase* inst_ptr)
    {
        std::lock_guard<std::mutex> lock(inst_pool_mutex_);
        if (idle_inst_map_.find(inst_ptr) != idle_inst_map_.end())
        {
            idle_inst_map_.erase(inst_ptr);
            inst_ptr->fini();
            inst_pool_cond_.notify_one();
            return ERR_SUCCESS;
        }else{
            LError("InstPool delete_inst failed");
            return ERR_INVALID_PARAM;
        }
    }

    int pull_inst(InstanceBase** inst_ptr)
    {
        std::unique_lock<std::mutex> lock(inst_pool_mutex_);

        inst_pool_cond_.wait(lock, [this]() { return this->idle_inst_map_.size() > 0; });
        {
            *inst_ptr = *(idle_inst_map_.begin());
            busy_inst_map_.insert(*inst_ptr);
            idle_inst_map_.erase(idle_inst_map_.begin());
            return ERR_SUCCESS;
        }
    }

    int push_inst(InstanceBase* inst_ptr)
    {
        std::lock_guard<std::mutex> lock(inst_pool_mutex_);
        if (busy_inst_map_.find(inst_ptr) != busy_inst_map_.end())
        {
            busy_inst_map_.erase(inst_ptr);
            idle_inst_map_.insert(inst_ptr);
            inst_pool_cond_.notify_one();
            return ERR_SUCCESS;
        }
        else
        {
            LError("InstPool push_inst failed, invalid inst_ptr");
            return ERR_INVALID_PARAM;
        }
    }

private:
    std::atomic<bool> initialized_{false};
    std::mutex inst_pool_mutex_;
    std::condition_variable inst_pool_cond_;
    std::unordered_set<InstanceBase*> idle_inst_map_;
    std::unordered_set<InstanceBase*> busy_inst_map_;
};