#ifndef AUDIO_CONTROLLER_H
#define AUDIO_CONTROLLER_H

#include "AudioTaskBuilder.h"
#include "Observer.h"
#include "Subject.h"
#include "ThreadPool.h"
#include "WaveformGenerator.h"

#include <unordered_map>
#include <vector>

class AudioController : public Subject {
public:
    using DispatchMap = std::unordered_map<WaveformType, std::function<std::vector<float>(const AudioTask&)>>;

    void play(const AudioTask&);

    AudioController(std::shared_ptr<ThreadPool>);

private:
    WaveformGenerator wf_gen;
    DispatchMap map;
    std::shared_ptr<ThreadPool> thread_pool;
};

#endif // AUDIO_CONTROLLER_H
