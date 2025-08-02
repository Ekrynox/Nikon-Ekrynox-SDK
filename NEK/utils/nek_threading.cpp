#include "nek_threading.hpp"



using namespace nek::utils;



//ThreadedClassBase
ThreadedClassBase::ThreadedClassBase() {
	running_ = true;
}

void ThreadedClassBase::sendTaskAsync(std::function<void()> task) {
	mutexTasks_.lock();

	tasks_.push(task);

	mutexTasks_.unlock();
	cvTasks_.notify_one();
}

void ThreadedClassBase::sendTask(std::function<void()> task) {
	std::promise<void> p;
	auto f = p.get_future();

	sendTaskAsync([&] { p.set_value(); task(); });

	f.get();
}

void ThreadedClassBase::threadTask() {
	while (running_) {
		mutexTasks_.lock();
		while (tasks_.size() > 0) {
			auto task = tasks_.front();
			tasks_.pop();
			mutexTasks_.unlock();

			task();

			mutexTasks_.lock();
		}
		mutexTasks_.unlock();

		std::unique_lock lk(mutexTasks_);
		cvTasks_.wait(lk, [this] { return !this->running_ || (this->tasks_.size() > 0); });
	}
}



//ThreadedClass
ThreadedClass::~ThreadedClass() {
	running_ = false;
	cvTasks_.notify_all();
	if (thread_.joinable()) {
		thread_.join();
	}
}

void ThreadedClass::startThread() {
	thread_ = std::thread([this] { this->threadTask(); });
}



//MultiThreadedClass
MultiThreadedClass::~MultiThreadedClass() {
	running_ = false;
	cvTasks_.notify_all();
	for (auto& thread : threads_) {
		if (thread.joinable()) {
			thread.join();
		}
	}
}

void MultiThreadedClass::startThread() {
	threads_.push_back(std::thread([this] { this->threadTask(); }));
}