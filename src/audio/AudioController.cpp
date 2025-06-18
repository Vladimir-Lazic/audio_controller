#include "AudioController.h"

AudioController::AudioController(std::shared_ptr<ThreadPool> pool)
    : thread_pool(std::move(pool))
{
    map = {
        { WaveformType::Sine, [this](const AudioTask& req) { return wf_gen.generateSineWave(req.frequency, req.amplitude, req.sample_rate, req.phase); } },
        { WaveformType::Square, [this](const AudioTask& req) { return wf_gen.generateSquare(req.frequency, req.amplitude, req.sample_rate, req.phase); } },
        { WaveformType::Sawtooth, [this](const AudioTask& req) { return wf_gen.generateSawtooth(req.frequency, req.amplitude, req.sample_rate, req.phase); } },
        { WaveformType::Triangle, [this](const AudioTask& req) { return wf_gen.generateTriangle(req.frequency, req.amplitude, req.sample_rate, req.phase); } },
        { WaveformType::WhiteNoise, [this](const AudioTask& req) { return wf_gen.generateWhiteNoise(req.frequency, req.amplitude, req.sample_rate); } }
    };
}

void AudioController::play(const AudioTask& task)
{
    thread_pool->loadTask([this, task = task]() {
        auto signal = map.at(task.waveform_type)(task);
        notify(signal);
    });
}
