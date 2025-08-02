#pragma once
#include "../nek.hpp"

#include <atomic>
#include <future>
#include <mutex>
#include <queue>
#include <thread>



namespace nek::utils {

	class NEK_API ThreadedClass {
	public:
		ThreadedClass();
		~ThreadedClass();

		void startThread();

		void sendTaskAsync(std::function<void()> task);
		void sendTask(std::function<void()> task);
		template<typename T> T sendTaskWithResult(std::function<T()> task) {
			std::promise<T> p;
			auto f = p.get_future();

			sendTaskAsync([&] { p.set_value(task()); });

			return f.get();
		}

	protected:
		void threadTask();

		std::queue<std::function<void()>> tasks_;
		std::thread thread_;
		std::atomic<bool> running_;
		std::mutex mutexTasks_;
		std::condition_variable cvTasks_;
	};

}