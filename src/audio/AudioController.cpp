#include "AudioController.h"
#include "RtAudioCallback.h"

#include <cmath>
#include <numbers>
#include <random>

AudioController::AudioController(std::shared_ptr<WorkerPool> workers, std::shared_ptr<WaveformBuffer> buffer)
    : worker_pool(std::move(workers))
    , waveform_buffer(std::move(buffer))
{
    if (dac.getDeviceCount() < 1) {
        throw std::runtime_error("No audio deviced found!");
    }

    map = {
        { WaveformType::Sine, [this](const AudioTask& t, float phase) { return SineWave(phase, t.frequency, t.amplitude); } },
        { WaveformType::Square, [this](const AudioTask& t, float phase) { return SquareWave(phase, t.frequency, t.amplitude); } },
        { WaveformType::Sawtooth, [this](const AudioTask& t, float phase) { return SawtoothWave(phase, t.frequency, t.amplitude); } },
        { WaveformType::Triangle, [this](const AudioTask& t, float phase) { return TriangleWave(phase, t.frequency, t.amplitude); } },
        { WaveformType::WhiteNoise, [this](const AudioTask& t, float /*phase*/) { return WhiteNoise(t.amplitude); } }
    };

    stream_parameters.deviceId = dac.getDefaultOutputDevice();
    stream_parameters.nChannels = Channels::Number;
    stream_parameters.firstChannel = 0;

    dac.openStream(&stream_parameters,
        nullptr,
        RTAUDIO_FLOAT32,
        Samples::Rate,
        &buffer_frames,
        audioCallback,
        reinterpret_cast<void*>(waveform_buffer.get()));

    dac.startStream();

    active_task.playback_state = PlaybackState::Stop;
    running = true;

    worker_pool->loadWorker([this]() {
        float current_phase = 0.0f;

        while (running) {
            AudioTask local_task;
            {
                threading::LockType lock(task_mutex);
                local_task = active_task;
            }

            float phase_increment = 2.0f * std::numbers::pi * local_task.frequency / sample_rate;
            float sample = 0.0f;

            if (local_task.playback_state == PlaybackState::Stop) {
                sample = 0.0f;
            } else {
                sample = map.find(local_task.waveform_type)->second(local_task, current_phase);
                current_phase += phase_increment;
                if (current_phase >= 2.0f * std::numbers::pi) {
                    current_phase -= 2.0f * std::numbers::pi;
                }
            }

            while (!waveform_buffer->push(sample)) {
                std::this_thread::yield();
            }

            notify(sample);

            auto sleep_time = std::chrono::microseconds(static_cast<int>(1'000'000.0 / sample_rate));
            std::this_thread::sleep_for(sleep_time);
        }
    });
}

AudioController::~AudioController()
{
    running = true;

    if (dac.isStreamRunning()) {
        dac.stopStream();
    }

    if (dac.isStreamOpen()) {
        dac.closeStream();
    }
}

void AudioController::stop()
{
    threading::LockType lock(task_mutex);
    active_task.playback_state = PlaybackState::Stop;
}

void AudioController::play(const AudioTask& task)
{
    threading::LockType lock(task_mutex);
    active_task = task;
}

float AudioController::SineWave(float phase, float /*frequency*/, float amplitude)
{
    return amplitude * std::sin(phase);
}

float AudioController::SquareWave(float phase, float /*frequency*/, float amplitude)
{
    return (std::sin(phase) >= 0.f) ? amplitude : -amplitude;
}

float AudioController::SawtoothWave(float phase, float frequency, float amplitude)
{
    float period = 2.0f * std::numbers::pi;
    float local_time = fmodf(phase, period) / period;
    return (2.0f * amplitude * local_time) - amplitude;
}

float AudioController::TriangleWave(float phase, float frequency, float amplitude)
{
    float period = 2.0f * std::numbers::pi;
    float local_time = fmodf(phase, period) / period;
    return (4.0f * amplitude) * std::abs(local_time - 0.5f) - amplitude;
}

float AudioController::WhiteNoise(float amplitude)
{
    static thread_local std::mt19937 gen(std::random_device {}());
    std::uniform_real_distribution<float> dis(-amplitude, amplitude);
    return dis(gen);
}