#include "WaveformGenerator.h"

#include <cmath>
#include <numbers>
#include <random>

std::vector<float> WaveformGenerator::generateSineWave(float frequency,
    float amplitude,
    int sample_rate,
    float phase)
{
    std::vector<float> sine_wave(sample_rate / frequency, 0.f);
    float time = 0.f;
    for (auto& sample : sine_wave) {
        sample = amplitude * std::sin(2 * std::numbers::pi * frequency * time + phase);
        time += 1 / sample_rate;
    }
    return sine_wave;
}

std::vector<float> WaveformGenerator::generateSquare(float frequency,
    float amplitude,
    int sample_rate,
    float phase)
{
    std::vector<float> square_wave(sample_rate / frequency, 0.f);
    float time = 0.f;
    for (auto& sample : square_wave) {
        sample = (std::sin(2 * std::numbers::pi * frequency * time + phase) >= 0.f) ? amplitude : -amplitude;
        time += 1 / sample_rate;
    }
    return square_wave;
}

std::vector<float> WaveformGenerator::generateSawtooth(float frequency,
    float amplitude,
    int sample_rate,
    float phase)
{
    std::vector<float> sawtooth(sample_rate / frequency, 0.f);
    float time = 0.f;
    for (auto& sample : sawtooth) {
        float local_time = fmodf(time + phase / (2 * std::numbers::pi * frequency), 1.f / frequency);
        sample = (2 * amplitude * frequency) * local_time - amplitude;
        time += 1 / sample_rate;
    }

    return sawtooth;
}

std::vector<float> WaveformGenerator::generateTriangle(float frequency,
    float amplitude,
    int sample_rate,
    float phase)
{
    std::vector<float> triangle(sample_rate / frequency, 0.f);
    float time = 0.f;
    for (auto& sample : triangle) {
        float local_time = fmodf(time + phase / (2 * std::numbers::pi * frequency), 1.f / frequency);
        sample = 4 * amplitude * frequency * std::abs(local_time - 0.5f / frequency) - amplitude;
        time += 1 / sample_rate;
    }

    return triangle;
}

std::vector<float> WaveformGenerator::generateWhiteNoise(float frequency,
    float amplitude,
    int sample_rate)
{
    std::vector<float> white_nosie(sample_rate / frequency, 0.f);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution dis(-amplitude, amplitude);

    for (auto& sample : white_nosie) {
        sample = dis(gen);
    }

    return white_nosie;
}
