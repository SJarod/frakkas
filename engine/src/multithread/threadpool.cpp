#include <iostream>

#include <chrono>

#include "debug/log.hpp"

#include "multithread/threadpool.hpp"

ThreadPool::ThreadPool(const unsigned int i_nThread)
{
	this->nThread = i_nThread;

	running.test_and_set();

	multithread = i_nThread > 0;

	for (unsigned int i = 0; i < i_nThread; ++i)
		threads.emplace_back(std::jthread(std::bind(&ThreadPool::PoolRoutine, this, i)));
}

ThreadPool::~ThreadPool()
{
	running.clear();
	queueCV.notify_all();
	for (auto& thread : threads)
		thread.join();
}

void ThreadPool::SetMultithread(const bool i_param)
{
	if (tasks.empty())
		multithread = i_param;
}

const bool ThreadPool::GetMultithreadParam() const
{
	return multithread;
}

unsigned int ThreadPool::GetThreadsNumber() const
{
	return nThread;
}

void ThreadPool::AddTask(const Task& i_fct)
{
	{
		std::lock_guard<std::mutex> guard(queueMX);

		if (tasks.empty())
			queueCV.notify_one();

		tasks.emplace_back(Task(i_fct));
	}

	if (!multithread)
		Work();
}

void ThreadPool::RemoveFirstTask()
{
	tasks.pop_front();
}

void ThreadPool::Work()
{
	Task t;

	if (multithread)
	{
		if (tasks.empty())
		{
			std::unique_lock<std::mutex> lock(queueMX);
			queueCV.wait(lock, [&]() {
				return running.test() ? !tasks.empty() : true;
				});

			return;
		}
		else
		{
			std::lock_guard<std::mutex> guard(queueMX);

			if (tasks.empty())
				return;

			t = tasks.front();
			RemoveFirstTask();
		}
	}
	else
	{
		t = tasks.front();
		RemoveFirstTask();
	}

	t(/*userData*/);

	auto now = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::steady_clock::now().time_since_epoch()).count());
	lastTaskTime.store(now);
}

void ThreadPool::PoolRoutine(const int i_id)
{
	while (running.test())
	{
		if (!multithread)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));

		Work();
	}
}

void ThreadPool::PrintThreadId(const int i_id)
{
	std::lock_guard<std::mutex> guard(printMX);
	Log::Info("thread #" + std::to_string(i_id));
}

void ThreadPool::SetStartTime()
{
	startTime = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::steady_clock::now().time_since_epoch()).count());
}

float ThreadPool::GetLoadingTime()
{
	endTime = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::steady_clock::now().time_since_epoch()).count());

	return (endTime - startTime) / 1000.f;
}

float ThreadPool::PrintLastWorkingTime()
{
	if (lastTaskTime == 0.f)
		return 0.f;

	return (lastTaskTime - startTime) / 1000.f;
}

float ThreadPool::PrintLastWorkingTimeOnce()
{
	if (lastTaskTime == 0.f)
		return 0.f;

	float val = (lastTaskTime - startTime) / 1000.f;

	std::lock_guard<std::mutex> guard(printMX);
	Log::Info("LOADING TIME : " + std::to_string(val) + "s");

	lastTaskTime = 0.f;

	return val;
}