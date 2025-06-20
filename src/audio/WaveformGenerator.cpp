#include "WaveformGenerator.h"

#include <cmath>
#include <numbers>
#include <random>

#include <iostream>

WaveformGenerator::WaveformGenerator()
    : audio_channel { 0 }
{
}

const WaveformBuffer& WaveformGenerator::generateSineWave(float frequency,
    float amplitude,
    int sample_rate,
    float phase)
{
    audio_channel.resize(default_buffer_size);

    float phase_increment = 2.0f * std::numbers::pi * frequency / sample_rate;
    float current_phase = phase;

    for (auto& sample : audio_channel) {
        sample = amplitude * std::sin(current_phase);
        current_phase += phase_increment;
        if (current_phase >= 2.0f * std::numbers::pi) {
            current_phase -= 2.0f * std::numbers::pi;
        }
        std::cout << sample << ";";
    }
    return audio_channel;
}

const WaveformBuffer& WaveformGenerator::generateSquare(float frequency,
    float amplitude,
    int sample_rate,
    float phase)
{
    audio_channel.resize(default_buffer_size);

    float phase_increment = 2.0f * std::numbers::pi * frequency / sample_rate;
    float current_phase = phase;

    for (auto& sample : audio_channel) {
        sample = (std::sin(current_phase) >= 0.f) ? amplitude : -amplitude;
        current_phase += phase_increment;
        if (current_phase >= 2.0f * std::numbers::pi) {
            current_phase -= 2.0f * std::numbers::pi;
        }
        std::cout << sample << ";";
    }
    return audio_channel;
}

const WaveformBuffer& WaveformGenerator::generateSawtooth(float frequency,
    float amplitude,
    int sample_rate,
    float phase)
{
    audio_channel.resize(default_buffer_size);

    float time = 0.0f;
    for (auto& sample : audio_channel) {
        float local_time = fmodf(time + phase / (2 * std::numbers::pi * frequency), 1.f / frequency);
        sample = (2 * amplitude * frequency) * local_time - amplitude;
        time += 1.0f / sample_rate;
        if (time >= (1.0f / frequency)) {
            time -= 1.0f / frequency;
        }
    }

    return audio_channel;
}

const WaveformBuffer& WaveformGenerator::generateTriangle(float frequency,
    float amplitude,
    int sample_rate,
    float phase)
{
    audio_channel.resize(default_buffer_size);

    float time = 0.f;
    for (auto& sample : audio_channel) {
        float local_time = fmodf(time + phase / (2 * std::numbers::pi * frequency), 1.f / frequency);
        sample = 4 * amplitude * frequency * std::abs(local_time - 0.5f / frequency) - amplitude;
        time += 1.0f / sample_rate;
        if (time >= (1.0f / frequency)) {
            time -= 1.0f / frequency;
        }
    }

    return audio_channel;
}

const WaveformBuffer& WaveformGenerator::generateWhiteNoise(float amplitude)
{
    audio_channel.resize(default_buffer_size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution dis(-amplitude, amplitude);

    for (auto& sample : audio_channel) {
        sample = dis(gen);
    }

    return audio_channel;
}
