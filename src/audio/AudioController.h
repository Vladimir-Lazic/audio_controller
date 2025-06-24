#ifndef AUDIO_CONTROLLER_H
#define AUDIO_CONTROLLER_H

#include "AudioTaskBuilder.h"
#include "Observer.h"
#include "Subject.h"
#include "WorkerPool.h"

#include "RtAudio.h"

#include <unordered_map>
#include <vector>

class AudioController : public Subject {
public:
    void play(const AudioTask& task);
    void stop();

    explicit AudioController(std::shared_ptr<WorkerPool>, std::shared_ptr<WaveformBuffer>);
    ~AudioController();

private:
    std::shared_ptr<WorkerPool> worker_pool;

    std::shared_ptr<WaveformBuffer> waveform_buffer;
    std::mutex buffer_mutex;

    std::atomic<bool> running { false };

    AudioTask active_task;
    std::mutex task_mutex;

    RtAudio dac;
    unsigned int buffer_frames { Buffer::Frames };
    RtAudio::StreamParameters stream_parameters;

    int sample_rate { Samples::Rate };
    DispatchMap map;

    float SineWave(float phase, float frequency, float amplitude);
    float SquareWave(float phase, float frequency, float amplitude);
    float SawtoothWave(float phase, float frequency, float amplitude);
    float TriangleWave(float phase, float frequency, float amplitude);
    float WhiteNoise(float amplitude);
};

#endif // AUDIO_CONTROLLER_H
