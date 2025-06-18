#include "WaveformGenerator.h"

#include <cmath>
#include <numbers>
#include <random>

const WaveformGenerator::Buffer& WaveformGenerator::generateSineWave(float frequency,
    float amplitude,
    int sample_rate,
    float phase)
{
    audio_channel.resize(static_cast<size_t>(sample_rate / frequency + 0.5f));
    float time = 0.f;
    for (auto& sample : audio_channel) {
        sample = amplitude * std::sin(2 * std::numbers::pi * frequency * time + phase);
        time += 1.0f / sample_rate;
    }
    return audio_channel;
}

const WaveformGenerator::Buffer& WaveformGenerator::generateSquare(float frequency,
    float amplitude,
    int sample_rate,
    float phase)
{
    audio_channel.resize(static_cast<size_t>(sample_rate / frequency + 0.5f));
    float time = 0.f;
    for (auto& sample : audio_channel) {
        sample = (std::sin(2 * std::numbers::pi * frequency * time + phase) >= 0.f) ? amplitude : -amplitude;
        time += 1.0f / sample_rate;
    }
    return audio_channel;
}

const WaveformGenerator::Buffer& WaveformGenerator::generateSawtooth(float frequency,
    float amplitude,
    int sample_rate,
    float phase)
{
    audio_channel.resize(static_cast<size_t>(sample_rate / frequency + 0.5f));
    float time = 0.f;
    for (auto& sample : audio_channel) {
        float local_time = fmodf(time + phase / (2 * std::numbers::pi * frequency), 1.f / frequency);
        sample = (2 * amplitude * frequency) * local_time - amplitude;
        time += 1.0f / sample_rate;
    }

    return audio_channel;
}

const WaveformGenerator::Buffer& WaveformGenerator::generateTriangle(float frequency,
    float amplitude,
    int sample_rate,
    float phase)
{
    audio_channel.resize(static_cast<size_t>(sample_rate / frequency + 0.5f));
    float time = 0.f;
    for (auto& sample : audio_channel) {
        float local_time = fmodf(time + phase / (2 * std::numbers::pi * frequency), 1.f / frequency);
        sample = 4 * amplitude * frequency * std::abs(local_time - 0.5f / frequency) - amplitude;
        time += 1.0f / sample_rate;
    }

    return audio_channel;
}

const WaveformGenerator::Buffer& WaveformGenerator::generateWhiteNoise(float frequency,
    float amplitude,
    int sample_rate)
{
    audio_channel.resize(static_cast<size_t>(sample_rate / frequency + 0.5f));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution dis(-amplitude, amplitude);

    for (auto& sample : audio_channel) {
        sample = dis(gen);
    }

    return audio_channel;
}
