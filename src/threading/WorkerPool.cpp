#include "WorkerPool.h"

WorkerPool::WorkerPool(size_t num_of_workers)
    : alive { true }
    , active_tasks_counter { 0 }
{
    for (size_t i = 0; i < num_of_workers; i++) {
        workers.emplace_back(&WorkerPool::worker, this);
    }
}

WorkerPool::~WorkerPool()
{
    {
        LockType lock(task_mutex);
        alive = false;
    }

    signal.notify_all();

    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void WorkerPool::loadTask(TaskType new_task)
{
    {
        LockType lock(task_mutex);
        tasks.push(std::move(new_task));
        active_tasks_counter++;
    }
    signal.notify_one();
}

void WorkerPool::worker()
{
    while (alive) {
        TaskType task;
        {
            LockType lock(task_mutex);
            signal.wait(lock, [this]() { return !tasks.empty() || !alive; });
            if (!alive && tasks.empty()) {
                return;
            }
            task = std::move(tasks.front());
            tasks.pop();
            active_tasks_counter--;
            if (active_tasks_counter == 0 && tasks.empty()) {
                signal.notify_all();
            }
        }
        task();
    }
}
