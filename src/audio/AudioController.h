#ifndef AUDIO_CONTROLLER_H
#define AUDIO_CONTROLLER_H

#include "AudioTaskBuilder.h"
#include "Observer.h"
#include "RtAudio.h"
#include "Subject.h"
#include "TaskPool.h"
#include "WaveformGenerator.h"
#include "WorkerPool.h"

#include <unordered_map>
#include <vector>

class AudioController : public Subject {
public:
    void query();

    explicit AudioController(std::shared_ptr<WorkerPool>, std::shared_ptr<TaskPool>);
    ~AudioController();

private:
    void play(const AudioTask& task);
    void stop();

    std::atomic<bool> alive { false };

    std::shared_ptr<WorkerPool> thread_pool;
    std::shared_ptr<TaskPool> task_pool;
    std::shared_ptr<AudioQueue> audio_queue;

    WaveformGenerator wf_gen;

    std::mutex audio_task_mutex;
    AudioTask audio_task;

    RtAudio dac;
    unsigned int buffer_frames { Buffer::Frames };
    RtAudio::StreamParameters stream_parameters;
};

#endif // AUDIO_CONTROLLER_H
