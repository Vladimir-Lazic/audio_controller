#ifndef AUDIO_CONTROLLER_H
#define AUDIO_CONTROLLER_H

#include "AudioTaskBuilder.h"
#include "Observer.h"
#include "Subject.h"
#include "WorkerPool.h"
#include "WaveformGenerator.h"

#include "RtAudio.h"

#include <unordered_map>
#include <vector>

class AudioController : public Subject {
public:
    using DispatchMap = std::unordered_map<WaveformType, std::function<std::vector<float>(const AudioTask&)>>;

    void play(const WaveformBuffer&) const;
    void stop() const;

    void processTask(const AudioTask&);

    const WaveformBuffer& getPlaybackBuffer() const;

    explicit AudioController(std::shared_ptr<WorkerPool>);
    ~AudioController();

private:
    std::shared_ptr<WorkerPool> thread_pool;

    mutable WaveformBuffer playback_buffer;
    WaveformGenerator wf_gen;
    DispatchMap map;

    mutable RtAudio dac;
    mutable unsigned int buffer_frames { Buffer::Frames };
    RtAudio::StreamParameters stream_parameters;
};

#endif // AUDIO_CONTROLLER_H
