#include "ThreadPool.h"

ThreadPool::ThreadPool(int32_t numOfWorkers)
    : alive { true }
    , activeTasksCounter { 0 }
{
    for (int32_t i = 0; i < numOfWorkers; i++) {
        workers.emplace_back(&ThreadPool::worker, this);
    }
}

ThreadPool::~ThreadPool()
{
    {
        LockType lock(taskMutex);
        signal.wait(lock, [this]() { return (activeTasksCounter == 0) && (tasks.empty()); });
        alive = false;
    }

    signal.notify_all();

    for (auto& worker : workers) {
        worker.join();
    }
}

void ThreadPool::loadTask(TaskType newTask)
{
    {
        LockType lock(taskMutex);
        tasks.push(newTask);
        activeTasksCounter++;
    }
    signal.notify_one();
}

void ThreadPool::worker()
{
    while (alive) {
        {
            LockType lock(taskMutex);
            signal.wait(lock, [this]() { return !tasks.empty() || !alive; });
            if (!alive && tasks.empty()) {
                return;
            }
            TaskType task = std::move(tasks.front());
            tasks.pop();
            activeTasksCounter--;
            if (activeTasksCounter == 0 && tasks.empty()) {
                signal.notify_all();
            }
            task();
        }
    }
}
