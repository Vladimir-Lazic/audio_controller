#ifndef TASK_POOL_H
#define TASK_POOL_H

#include "AudioDefinitions.h"
#include "Threading.h"

#include <mutex>
#include <optional>
#include <queue>

class TaskPool {
public:
    void processRequest(const std::string&);
    std::optional<AudioTask> query();

private:
    std::mutex task_mutex {};
    std::queue<AudioTask> tasks {};
};

#endif // TASK_POOL_H
