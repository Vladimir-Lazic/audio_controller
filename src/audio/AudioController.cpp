#include "AudioController.h"

AudioController::AudioController()
{
    map = {
        { WaveformType::Sine, [this](const AudioTask& req) { return wf_gen.generateSineWave(req.frequency, req.amplitude, req.sample_rate, req.phase); } },
        { WaveformType::Square, [this](const AudioTask& req) { return wf_gen.generateSquare(req.frequency, req.amplitude, req.sample_rate, req.phase); } },
        { WaveformType::Sawtooth, [this](const AudioTask& req) { return wf_gen.generateSawtooth(req.frequency, req.amplitude, req.sample_rate, req.phase); } },
        { WaveformType::Triangle, [this](const AudioTask& req) { return wf_gen.generateTriangle(req.frequency, req.amplitude, req.sample_rate, req.phase); } },
        { WaveformType::WhiteNoise, [this](const AudioTask& req) { return wf_gen.generateWhiteNoise(req.frequency, req.amplitude, req.sample_rate); } }
    };
}

void AudioController::play(const AudioTask& request)
{
    if (map.contains(request.waveform_type)) {
        thread_pool.loadTask(std::move([this, request]() {
            auto signal = map.at(request.waveform_type)(request);
            notify();
        }));
    }
}
