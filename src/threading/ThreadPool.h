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

    ThreadPool(int32_t numOfWorkers);

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    ThreadPool(ThreadPool&&) = default;
    ThreadPool& operator=(ThreadPool&&) = default;

    ~ThreadPool();

    void loadTask(TaskType newTask);

private:
    bool alive { false };
    std::mutex taskMutex {};
    std::condition_variable signal {};
    std::vector<std::thread> workers {};
    std::queue<TaskType> tasks {};
    std::atomic<int> activeTasksCounter {};

    void worker();
};

#endif // THREAD_POOL_H
