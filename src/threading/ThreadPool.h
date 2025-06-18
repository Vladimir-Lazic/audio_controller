#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
public:
    using LockType = std::unique_lock<std::mutex>;
    using TaskType = std::function<void()>;

    ThreadPool(size_t);

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    ~ThreadPool();

    void loadTask(TaskType);

private:
    bool alive { false };
    std::mutex task_mutex {};
    std::condition_variable signal {};
    std::vector<std::thread> workers {};
    std::queue<TaskType> tasks {};
    std::atomic<int> active_tasks_counter {};

    void worker();
};

#endif // THREAD_POOL_H
