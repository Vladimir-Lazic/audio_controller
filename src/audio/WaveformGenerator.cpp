#include "WaveformGenerator.h"
#include "Threading.h"

#include <cmath>
#include <numbers>
#include <random>

WaveformGenerator::WaveformGenerator(std::shared_ptr<AudioQueue> queue)
    : audio_queue(std::move(queue))
{
    map = {
        { WaveformType::Sine, [this](const AudioTask& req) { SineWave(req.frequency, req.amplitude, req.phase); } },
        { WaveformType::Square, [this](const AudioTask& req) { Square(req.frequency, req.amplitude, req.phase); } },
        { WaveformType::Sawtooth, [this](const AudioTask& req) { Sawtooth(req.frequency, req.amplitude, req.phase); } },
        { WaveformType::Triangle, [this](const AudioTask& req) { Triangle(req.frequency, req.amplitude, req.phase); } },
        { WaveformType::WhiteNoise, [this](const AudioTask& req) { WhiteNoise(req.amplitude); } }
    };
}

void WaveformGenerator::generate(const AudioTask& task)
{
    map.at(task.waveform_type)(task);
}

void WaveformGenerator::SineWave(float frequency,
    float amplitude,
    float phase)
{
    float phase_increment = 2.0f * std::numbers::pi * frequency / sample_rate;
    float current_phase = phase;

    for (int i = 0; i < Buffer::Frames; i++) {
        float sample = amplitude * std::sin(current_phase);
        while (!audio_queue->push(sample)) {
            std::this_thread::yield();
        }
        current_phase += phase_increment;
        if (current_phase >= 2.0f * std::numbers::pi) {
            current_phase -= 2.0f * std::numbers::pi;
        }
    }
}

void WaveformGenerator::Square(float frequency,
    float amplitude,
    float phase)
{
    float phase_increment = 2.0f * std::numbers::pi * frequency / sample_rate;
    float current_phase = phase;

    for (int i = 0; i < Buffer::Frames; i++) {
        float sample = (std::sin(current_phase) >= 0.f) ? amplitude : -amplitude;
        while (!audio_queue->push(sample)) {
            std::this_thread::yield();
        }
        current_phase += phase_increment;
        if (current_phase >= 2.0f * std::numbers::pi) {
            current_phase -= 2.0f * std::numbers::pi;
        }
    }
}

void WaveformGenerator::Sawtooth(float frequency,
    float amplitude,
    float phase)
{
    float time = 0.0f;
    for (int i = 0; i < Buffer::Frames; i++) {
        float local_time = fmodf(time + phase / (2 * std::numbers::pi * frequency), 1.f / frequency);
        float sample = (2 * amplitude * frequency) * local_time - amplitude;
        while (!audio_queue->push(sample)) {
            std::this_thread::yield();
        }
        time += 1.0f / sample_rate;
        if (time >= (1.0f / frequency)) {
            time -= 1.0f / frequency;
        }
    }
}

void WaveformGenerator::Triangle(float frequency,
    float amplitude,
    float phase)
{
    float time = 0.f;
    for (int i = 0; i < Buffer::Frames; i++) {
        float local_time = fmodf(time + phase / (2 * std::numbers::pi * frequency), 1.f / frequency);
        float sample = 4 * amplitude * frequency * std::abs(local_time - 0.5f / frequency) - amplitude;
        while (!audio_queue->push(sample)) {
            std::this_thread::yield();
        }
        time += 1.0f / sample_rate;
        if (time >= (1.0f / frequency)) {
            time -= 1.0f / frequency;
        }
    }
}

void WaveformGenerator::WhiteNoise(float amplitude)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution dis(-amplitude, amplitude);

    for (int i = 0; i < Buffer::Frames; i++) {
        float sample = dis(gen);
        while (!audio_queue->push(sample)) {
            std::this_thread::yield();
        }
    }
}
