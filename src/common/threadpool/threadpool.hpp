#pragma once
#include "taskqueue.hpp"
#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
class ThreadPool
{
private:
    class ThreadWorker
    {
    private:
        int m_id;
        ThreadPool *m_pool;

    public:
        ThreadWorker(ThreadPool *pool, const int id) : m_id(id), m_pool(pool)
        {
        }

        void operator()()
        {
            std::function<void()> func;
            bool dequeued;

            while (!m_pool->shutdown_)
            {
                {
                    std::unique_lock<std::mutex> lock(
                        m_pool->conditional_mutex_);
                    if (m_pool->queue_.is_empty())
                    {
                        m_pool->conditional_lock_.wait(lock);
                    }
                    dequeued = m_pool->queue_.pop(func);
                }
                if (dequeued)
                {
                    func();
                }
            }
        }
    };

public:
    ThreadPool(const int n_threads = 4)
        : shutdown_(false), threads_(std::vector<std::thread>(n_threads)){};

    ThreadPool(const ThreadPool &) = default;
    ThreadPool(ThreadPool &&) = default;
    ThreadPool &operator=(const ThreadPool &) = default;
    ThreadPool &operator=(ThreadPool &&) = default;

    void init()
    {
        for (int i = 0; i < threads_.size(); ++i)
        {
            threads_.at(i) = std::thread(ThreadWorker(this, i));
        }
    }

    void shutdown()
    {
        shutdown_ = true;
        conditional_lock_.notify_all();
        for (auto &thread : threads_)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
    }

    template <typename F, typename... Args>
    auto submit(F &&f, Args &&...args) -> std::future<decltype(f(args...))>
    {
        // Create a function with bounded parameter ready to execute
        std::function<decltype(f(args...))()> func = std::bind(
            std::forward<F>(f),
            std::forward<Args>(
                args)...); // 连接函数和参数定义，特殊函数类型，避免左右值错误
        // Encapsulate it into a shared pointer in order to be able to copy construct
        auto task_ptr =
            std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
        // Warp packaged task into void function
        std::function<void()> warpper_func = [task_ptr]() { (*task_ptr)(); };
        // 队列通用安全封包函数，并压入安全队列
        queue_.push(warpper_func);
        // 唤醒一个等待中的线程
        conditional_lock_.notify_one();
        // 返回先前注册的任务指针
        return task_ptr->get_future();
    }

private:
    bool shutdown_ = false;
    SafeQueue<std::function<void()>> queue_;
    std::vector<std::thread> threads_;
    std::mutex conditional_mutex_;
    std::condition_variable conditional_lock_;
};