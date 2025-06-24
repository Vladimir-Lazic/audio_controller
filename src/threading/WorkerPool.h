#ifndef WORKER_POOL_H
#define WORKER_POOL_H

#include <atomic>
#include <condition_variable>
#include <queue>
#include <thread>
#include <vector>

#include "Threading.h"

using namespace threading;

class WorkerPool {
public:
    void loadWorker(TaskType);

    WorkerPool(size_t);

    WorkerPool(const WorkerPool&) = delete;
    WorkerPool& operator=(const WorkerPool&) = delete;

    WorkerPool(WorkerPool&&) = delete;
    WorkerPool& operator=(WorkerPool&&) = delete;

    ~WorkerPool();

private:
    bool alive { false };
    std::mutex task_mutex {};
    std::condition_variable signal {};
    std::vector<std::thread> workers {};
    std::queue<TaskType> tasks {};
    std::atomic<int> active_tasks_counter {};

    void worker();
};

#endif // WORKER_POOL_H
