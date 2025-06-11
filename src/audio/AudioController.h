#ifndef AUDIO_CONTROLLER_H
#define AUDIO_CONTROLLER_H

#include "AudioTaskBuilder.h"
#include "Logger.h"
#include "Observer.h"
#include "Subject.h"
#include "ThreadPool.h"
#include "WaveformGenerator.h"
#include <unordered_map>
#include <vector>

class AudioController : public Subject {
public:
    using DispatchMap = std::unordered_map<WaveformType, std::function<std::vector<float>(const AudioTask&)>>;

    void play(const AudioTask& request);
    void pause(const AudioTask& requests);
    void stop();

    AudioController();

private:
    WaveformGenerator wf_gen;
    DispatchMap map;

    enum Threads : int32_t { Size = 5 };
    ThreadPool thread_pool{Threads::Size};
};

#endif // AUDIO_CONTROLLER_H
