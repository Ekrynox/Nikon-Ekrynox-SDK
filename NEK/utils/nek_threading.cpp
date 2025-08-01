#include "nek_threading.hpp"



using namespace nek::utils;



ThreadedClass::ThreadedClass() {
	running_ = true;
}

ThreadedClass::~ThreadedClass() {
	running_ = false;
	cvTasks_.notify_all();
	if (thread_.joinable()) {
		thread_.join();
	}
}

void ThreadedClass::threadTask() {
	while (running_) {
		mutexTasks_.lock();
		while (tasks_.size() > 0) {
			auto task = tasks_.front();
			tasks_.pop();
			mutexTasks_.unlock();

			mutexDevice_.lock();
			task();
			mutexDevice_.unlock();

			mutexTasks_.lock();
		}
		mutexTasks_.unlock();

		std::unique_lock lk(mutexTasks_);
		cvTasks_.wait(lk, [this] { return !this->running_ || (this->tasks_.size() > 0); });
	}
}

void ThreadedClass::sendTaskAsync(std::function<void()> task) {
	mutexTasks_.lock();

	tasks_.push(task);

	mutexTasks_.unlock();
	cvTasks_.notify_one();
}

void ThreadedClass::sendTask(std::function<void()> task) {
	std::promise<void> p;
	auto f = p.get_future();

	sendTaskAsync([&] { p.set_value(); task(); });

	f.get();
}