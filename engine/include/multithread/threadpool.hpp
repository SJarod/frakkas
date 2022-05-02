#pragma once

#include <thread>
#include <atomic>
#include <deque>
#include <mutex>
#include <condition_variable>

#include "multithread/task.hpp"

class ThreadPool
{
private:
	std::vector<std::jthread> threads;
	//number of threads
	unsigned int		nThread = 0;
	bool				multithread = false;

	float				startTime = 0.f;
	float				endTime = 0.f;
	std::atomic<float>	lastTaskTime;

	// shared data
	std::deque<Task>	tasks;

	std::mutex				queueMX;
	std::condition_variable queueCV;

	//is the routine running?
	std::atomic_flag running = ATOMIC_FLAG_INIT;

public:
	/**
	 * Create the thread pool.
	 * Specify the number of threads, the number of threads is by default the maximum that the machine can get.
	 *
	 * @param nThread
	 */
	ThreadPool(const unsigned int i_nThread = std::thread::hardware_concurrency());

	/**
	 * End the thread pool.
	 *
	 */
	~ThreadPool();

	/**
	 * Set the pool to function or not.
	 * true : multithread
	 * false : monothread
	 *
	 * @param i_param
	 */
	void SetMultithread(const bool i_param);

	/**
	 * Is the pool functionning in multithread?
	 *
	 * @return
	 */
	const bool GetMultithreadParam() const;

	/**
	 * Get the number of threads.
	 *
	 * @return
	 */
	unsigned int GetThreadsNumber() const;

	/**
	 * Add a task to the task queue.
	 *
	 * @param fct
	 */
	void AddTask(const Task& i_fct);

	/**
	 * Remove the first task from the task queue.
	 *
	 */
	void RemoveFirstTask();

	/**
	 * Choose a task then do it.
	 *
	 */
	void Work();

	/**
	 * Thread routine.
	 * Main loop function
	 *
	 */
	void PoolRoutine(const int i_id);

	/**
	 * Print the thread id.
	 *
	 */
	void PrintThreadId(const int i_id);

	/**
	 * Get the time to store as the thread pool's starting time.
	 *
	 */
	void SetStartTime();

	/**
	 * Return the time between now and the start time.
	 * Equivalent to a loading time.
	 * The user chooses when to show the loading time, do not forget to set the start time.
	 */
	float GetLoadingTime();

	/**
	 * Time when the pool has finished doing every tasks.
	 */
	float PrintLastWorkingTime();

	/**
	 * Print the time when the last task was finished.
	 *
	 * @return
	 */
	float PrintLastWorkingTimeOnce();
};