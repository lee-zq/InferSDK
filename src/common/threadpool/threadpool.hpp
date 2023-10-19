#pragma once
#include <vector>
#include <thread>
#include <functional>
#include <condition_variable>
#include <future>
#include <atomic>
#include <memory>
#include <iostream>

#include "taskqueue.hpp"
class ThreadPool
{
private:
	class ThreadWorker 
       {
	private:
		int m_id;
		ThreadPool* m_pool;
	public:
		ThreadWorker(ThreadPool* pool, const int id) :
			m_id(id), m_pool(pool) {}

		void operator()() {
			std::function<void()> func;
			bool dequeued;

			while (!m_pool->m_shutdown)
			{
				{
					std::unique_lock<std::mutex> lock(m_pool->m_conditional_mutex);
					if (m_pool->m_queue.is_empty())
					{
						m_pool->m_conditional_lock.wait(lock);
					}
					dequeued = m_pool->m_queue.pop(func);
				}
				if (dequeued)
				{
					func();
				}
			}
		}
	};

public:
	ThreadPool(const int n_threads = 4) :m_threads(std::vector<std::thread>(n_threads)), m_shutdown(false) {}
	ThreadPool(const ThreadPool&) = default;
	ThreadPool(ThreadPool&&) = default;
	ThreadPool& operator=(const ThreadPool&) = default;
	ThreadPool& operator=(ThreadPool&&) = default;

	void init() {
		for (int i = 0; i < m_threads.size(); ++i) {
			m_threads.at(i) = std::thread(ThreadWorker(this, i));
		}
	}

	void shutdown()
	{
		m_shutdown = true;
		m_conditional_lock.notify_all();
		for (int i = 0; i < m_threads.size(); i++)
		{
			if (m_threads.at(i).joinable()) {
				m_threads.at(i).join();
			}
		}
	}

	template <typename F, typename... Args>
	auto submit(F&& f, Args &&...args) -> std::future<decltype(f(args...))>
	{
		// Create a function with bounded parameter ready to execute
		std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);// 连接函数和参数定义，特殊函数类型，避免左右值错误
			// Encapsulate it into a shared pointer in order to be able to copy construct
		auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
		// Warp packaged task into void function
		std::function<void()> warpper_func = [task_ptr]()
		{
			(*task_ptr)();
		};
		// 队列通用安全封包函数，并压入安全队列
		m_queue.push(warpper_func);
		// 唤醒一个等待中的线程
		m_conditional_lock.notify_one();
		// 返回先前注册的任务指针
		return task_ptr->get_future();
	}
private:
	bool m_shutdown;
	SafeQueue<std::function<void()>> m_queue;
	std::vector<std::thread> m_threads;
	std::mutex m_conditional_mutex;
	std::condition_variable m_conditional_lock;
};