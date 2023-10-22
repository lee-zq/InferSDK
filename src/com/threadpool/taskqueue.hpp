#pragma once
#include <mutex>
#include <queue>

template <typename T>
class SafeQueue
{
public:
    SafeQueue()
    {
    }
    ~SafeQueue()
    {
    }
    SafeQueue(SafeQueue&& other)
    {
    }

    //获取empty状态
    bool is_empty()
    {
        std::lock_guard<std::mutex> m_guard(m);
        return task_queue_.empty();
    }

    // 入队和出队
    int push(T& task)
    {
        std::lock_guard<std::mutex> m_guard(m);
        task_queue_.push(task);
        return 0;
    }
    bool pop(T& task)
    {
        std::lock_guard<std::mutex> m_guard(m);
        if (task_queue_.empty())
        {
            return false;
        }
        task = task_queue_.front();
        task_queue_.pop();
        return true;
    }

private:
    std::queue<T> task_queue_;
    std::mutex m;
};