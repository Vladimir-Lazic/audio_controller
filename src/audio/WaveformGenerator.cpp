#include "WaveformGenerator.h"

#include <cmath>
#include <numbers>
#include <random>

#include <iostream>

WaveformGenerator::WaveformGenerator(WaveformBuffer& buffer, size_t buffer_size_, size_t sample_rate_)
    : buffer_size { buffer_size_ }
    , sample_rate { sample_rate_ }
    , playback_buffer(buffer)
{
}

const WaveformBuffer& WaveformGenerator::generateSineWave(float frequency,
    float amplitude,
    float phase)
{
    playback_buffer.resize(buffer_size);

    float phase_increment = 2.0f * std::numbers::pi * frequency / sample_rate;
    float current_phase = phase;

    for (auto& sample : playback_buffer) {
        sample = amplitude * std::sin(current_phase);
        current_phase += phase_increment;
        if (current_phase >= 2.0f * std::numbers::pi) {
            current_phase -= 2.0f * std::numbers::pi;
        }
        std::cout << sample << ";";
    }
    return playback_buffer;
}

const WaveformBuffer& WaveformGenerator::generateSquare(float frequency,
    float amplitude,
    float phase)
{
    playback_buffer.resize(buffer_size);

    float phase_increment = 2.0f * std::numbers::pi * frequency / sample_rate;
    float current_phase = phase;

    for (auto& sample : playback_buffer) {
        sample = (std::sin(current_phase) >= 0.f) ? amplitude : -amplitude;
        current_phase += phase_increment;
        if (current_phase >= 2.0f * std::numbers::pi) {
            current_phase -= 2.0f * std::numbers::pi;
        }
        std::cout << sample << ";";
    }
    return playback_buffer;
}

const WaveformBuffer& WaveformGenerator::generateSawtooth(float frequency,
    float amplitude,
    float phase)
{
    playback_buffer.resize(buffer_size);

    float time = 0.0f;
    for (auto& sample : playback_buffer) {
        float local_time = fmodf(time + phase / (2 * std::numbers::pi * frequency), 1.f / frequency);
        sample = (2 * amplitude * frequency) * local_time - amplitude;
        time += 1.0f / sample_rate;
        if (time >= (1.0f / frequency)) {
            time -= 1.0f / frequency;
        }
    }

    return playback_buffer;
}

const WaveformBuffer& WaveformGenerator::generateTriangle(float frequency,
    float amplitude,
    float phase)
{
    playback_buffer.resize(buffer_size);

    float time = 0.f;
    for (auto& sample : playback_buffer) {
        float local_time = fmodf(time + phase / (2 * std::numbers::pi * frequency), 1.f / frequency);
        sample = 4 * amplitude * frequency * std::abs(local_time - 0.5f / frequency) - amplitude;
        time += 1.0f / sample_rate;
        if (time >= (1.0f / frequency)) {
            time -= 1.0f / frequency;
        }
    }

    return playback_buffer;
}

const WaveformBuffer& WaveformGenerator::generateWhiteNoise(float amplitude)
{
    playback_buffer.resize(buffer_size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution dis(-amplitude, amplitude);

    for (auto& sample : playback_buffer) {
        sample = dis(gen);
    }

    return playback_buffer;
}
