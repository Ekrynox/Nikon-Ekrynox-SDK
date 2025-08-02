#pragma once
#include "../nek.hpp"

#include <atomic>
#include <future>
#include <mutex>
#include <queue>
#include <thread>



namespace nek::utils {

	class NEK_API ThreadedClassBase {
	public:
		ThreadedClassBase();

		virtual void startThread() = 0;

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
		std::atomic<bool> running_;
		std::mutex mutexTasks_;
		std::condition_variable cvTasks_;
	};



	class NEK_API ThreadedClass : public ThreadedClassBase {
	public:
		ThreadedClass() {};
		~ThreadedClass();

		void startThread() override;

	protected:
		std::thread thread_;
	};



	class NEK_API MultiThreadedClass : public ThreadedClassBase {
	public:
		MultiThreadedClass() {};
		~MultiThreadedClass();

		void startThread() override;

	protected:
		std::vector<std::thread> threads_;
	};

}