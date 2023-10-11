// #include<functional>
// #include<mutex>
// #include<vector>
// #include<D:\code\c++project\C++\src\project\safequeue.c++>
// #include<thread>
// #include<condition_variable>
// #include<future>
// class threadpool
// {
//     private:
//     class threadworker
//     {
//        private:
//        int m_id;
//        threadpool *m_pool;

//        public:
//        threadworker(const int id,threadpool *pool):m_id(id),m_pool(pool){}

//        void operator()()//重载操作
//        {
//             std::function<void()> func;
//             bool dequeued;
//             if(!m_pool->m_shutdown)
//             {
//                 std::unique_lock<std::mutex>lock(m_pool->m_conditional_mutex);
//                 if(m_pool->m_queue.empty())
//                 {
//                     m_pool->m_conditional_lock.wait(lock);
//                 }
//                 dequeued=m_pool->m_queue.dequeue(func);
//             }
//             if(dequeued)
//             {
//                 func();
//             }
//        }
//     }; 
//        bool m_shutdown;
//        std::vector<std::thread> m_threads;
//        safequeue<std::function<void()>> m_queue;
//        std::mutex m_conditional_mutex;
//        std::condition_variable m_conditional_lock;
    
//     public:
//     threadpool(const int n_threads=4):m_threads(std::vector<std::thread>(n_threads)),m_shutdown(false){}

//     threadpool(const threadpool &)=delete;
//     threadpool(threadpool &&)=delete;
//     threadpool &operator=(const threadpool &)=delete;
//     threadpool &&operator=(threadpool &&)=delete;

//     void init()//初始化分配线程
//     {
//         for(int i=0;i<m_threads.size();i++)
//         {
//             m_threads.at(i)=std::thread(threadworker(i,this));
//         }
//     }

//     void shutdown()//关闭线程
//     {
//         m_shutdown=true;
//         m_conditional_lock.notify_all();
//         for(int i=0;i<m_threads.size();i++)
//         {
//               if( m_threads.at(i).joinable())
//               {
//                   m_threads.at(i).join();
//               }
//         }
//     }
    
//     template<typename F,typename... Args>
//     auto submit(F &&f,Args &&...args)->std::future<decltype(f(args...))>
//     {
// //这段C++代码定义了一个名为`submit`的函数模板。它接受一个可调用对象`f`和一系列参数`args`。

// //函数模板使用了右值引用和可变参数模板的特性。`F &&f`表示对可调用对象`f`进行右值引用，`Args &&...args`表示可变数量的参数`args`，它们都是右值引用。

// //返回类型使用了`std::future<decltype(f(args...))>`，表示返回一个`std::future`对象，该对象的类型是通过调用`f(args...)`来推断的。

// //函数体中没有具体的实现，因此代码块中的大括号是空的。这意味着在使用这个函数模板时，需要根据具体的需求来提供实现。

// //这段代码的目的是定义一个通用的函数模板，用于提交任务并返回一个`std::future`对象，以便在将来某个时刻获取任务的结果。通过使用右值引用和可变参数模板，可以接受不同类型的可调用对象和参数，并返回相应的`std::future`对象。

// //总而言之，这段代码定义了一个通用的函数模板`submit`，用于提交任务并返回一个`std::future`对象，以便在将来获取任务的结果。具体的实现需要根据具体的需求来提供。

//          std::function<decltype(f(args...))()> func=std::bind(std::forward<F>(f),std::forward<Args>(args)...);//forward为完美转发


//          auto task_ptr=std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
         
//          std::function<void()>task=[task_ptr]()
//          {
//                (*task_ptr)();
//          };
// //这段C++代码创建了一个`std::function`对象`wrapper_func`，并使用Lambda表达式作为其可调用对象。

// //Lambda表达式`[task_ptr]() { (*task_ptr)(); }`定义了一个匿名函数，没有参数，返回类型为`void`。在函数体中，通过解引用`task_ptr`，调用了指针所指向的可调用对象。

// //这个Lambda表达式被用作初始化`std::function<void()>`对象`wrapper_func`，因此`wrapper_func`成为了一个可调用对象，可以像函数一样被调用。

// //这段代码的作用是将一个指针`task_ptr`所指向的可调用对象进行封装，并通过`std::function`对象`wrapper_func`来调用该可调用对象。通过这种方式，可以将一个具体的任务对象包装成一个可调用对象，并进行进一步的处理和调用。

// //总而言之，这段代码创建了一个`std::function`对象，并使用Lambda表达式将一个指针所指向的可调用对象进行封装。这样可以通过`std::function`对象来调用该可调用对象，并进行进一步的处理。
//          m_queue.enqueue(task);

//          m_conditional_lock.notify_one();

//          return task_ptr->get_future();
//     }
// };
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
		m_queue.enqueue(warpper_func);
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