#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <future>

template <typename F, typename... Ts>
inline auto runAsync(F&& f, Ts&&... params)
{
    return std::async(std::launch::async,
        std::forward<F>(f),
        std::forward<Ts>(params)...);
}

class ThreadPool {
public:
    using LockType = std::unique_lock<std::mutex>;
    using TaskType = std::function<void()>;

    void loadTask(TaskType);

    ThreadPool(size_t);

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    ~ThreadPool();

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
