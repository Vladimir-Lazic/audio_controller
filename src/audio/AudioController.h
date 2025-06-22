#ifndef AUDIO_CONTROLLER_H
#define AUDIO_CONTROLLER_H

#include "AudioTaskBuilder.h"
#include "Observer.h"
#include "RtAudio.h"
#include "Subject.h"
#include "WaveformGenerator.h"
#include "WorkerPool.h"

#include <unordered_map>
#include <vector>

class AudioController : public Subject {
public:
    void play(const WaveformBuffer&) const;
    void stop() const;

    void processTask(const AudioTask&);

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
