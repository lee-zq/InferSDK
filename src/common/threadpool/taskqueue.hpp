#pragma once
#include <queue>
#include <mutex>

template <typename T>
class SafeQueue
{
public:
	SafeQueue() {}
	~SafeQueue() {}
	SafeQueue(SafeQueue&& other) {}

	//获取empty状态
	bool is_empty() {
		std::lock_guard<std::mutex> m_guard(m);
		return taskQ_.empty();
	}

	// 入队和出队
	int push(T& task) {
		std::lock_guard<std::mutex> m_guard(m);
		taskQ_.push(task);
		return 0;
	}
	bool pop(T& task) {
		std::lock_guard<std::mutex> m_guard(m);
		if (taskQ_.empty()) {
			return false;
		}
		task = taskQ_.front();
		taskQ_.pop();
		return true;
	}

private:
	std::queue<T> taskQ_;
	std::mutex m;
};