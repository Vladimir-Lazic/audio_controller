#include "TaskPool.h"
#include "AudioTaskBuilder.h"
#include "Utils.h"
#include "WorkerPool.h"

using namespace threading;

void TaskPool::loadTask(const std::string& request)
{
    auto token = ';';
    auto parsed_commands = StringUtils::split(request, token);

    if (parsed_commands.size() != 5) {
        return;
    }

    auto task = AudioTaskBuilder::create()
                    .withPlaybackState(static_cast<PlaybackState>(std::stoi(parsed_commands[0])))
                    .withWaveform(static_cast<WaveformType>(std::stoi(parsed_commands[1])))
                    .withFrequency(std::stof(parsed_commands[2]))
                    .withAmplitude(std::stof(parsed_commands[3]))
                    .withPhase(std::stof(parsed_commands[4]))
                    .build();

    LockType lock(task_mutex);
    tasks.push(std::move(task));
}

std::optional<AudioTask> TaskPool::getTask()
{
    LockType lock(task_mutex);
    if (tasks.empty()) {
        return std::nullopt;
    }

    auto task = std::move(tasks.front());
    tasks.pop();
    return task;
}
