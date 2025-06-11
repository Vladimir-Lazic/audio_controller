#include "WaveformGenerator.h"
#include <cmath>
#include <gtest/gtest.h>
#include <numbers>

class WaveformGeneratorTestFixture : public ::testing::Test {
protected:
    void SetUp() override { };
    void TearDown() override { };

public:
    float freq = 440.f;
    float amplitude = 0.5f;
    float phase = 0.f;
    int sample_rate = 44100;

    WaveformGenerator wf_gen;
};

TEST_F(WaveformGeneratorTestFixture, TestSineWave)
{
    auto sine_wave = wf_gen.generateSineWave(freq, amplitude, sample_rate, phase);

    auto period_samples = static_cast<size_t>(sample_rate / freq);
    ASSERT_TRUE(period_samples == sine_wave.size());

    float time = 0.f;
    for (const auto& sample : sine_wave) {
        auto point = amplitude * std::sin(2 * std::numbers::pi * freq * time + phase);
        ASSERT_TRUE(sample == point);
        time += 1 / sample_rate;
    }
}

TEST_F(WaveformGeneratorTestFixture, TestSquareWave)
{
    auto square_wave = wf_gen.generateSquare(freq, amplitude, sample_rate, phase);

    auto period_samples = static_cast<size_t>(sample_rate / freq);
    ASSERT_TRUE(period_samples == square_wave.size());

    float time = 0.f;
    for (const auto& sample : square_wave) {
        float edge = (std::sin(2 * std::numbers::pi * freq * time + phase)) >= 0.f ? amplitude : -amplitude;
        ASSERT_TRUE(sample == edge);
        time += 1 / sample_rate;
    }
}

TEST_F(WaveformGeneratorTestFixture, TestSawtoothWave)
{
    auto sawtooth = wf_gen.generateSawtooth(freq, amplitude, sample_rate, phase);

    auto period_samples = static_cast<size_t>(sample_rate / freq);
    ASSERT_TRUE(period_samples == sawtooth.size());

    float time = 0.f;
    for (const auto& sample : sawtooth) {
        float local_time = fmodf(time + phase / (2 * std::numbers::pi * freq), 1.f / freq);
        float edge = (2 * amplitude * freq) * local_time - amplitude;
        ASSERT_TRUE(sample == edge);
        time += 1 / sample_rate;
    }
}

TEST_F(WaveformGeneratorTestFixture, TestTriangleWave)
{
    auto triangle = wf_gen.generateTriangle(freq, amplitude, sample_rate, phase);

    auto period_samples = static_cast<size_t>(sample_rate / freq);
    ASSERT_TRUE(period_samples == triangle.size());

    float time = 0.f;
    for (const auto& sample : triangle) {
        float local_time = fmodf(time + phase / (2 * std::numbers::pi * freq), 1.f / freq);
        float edge = (4 * amplitude * freq) * std::abs(local_time - 0.5f / freq) - amplitude;
        ASSERT_TRUE(sample == edge);
        time += 1 / sample_rate;
    }
}

TEST_F(WaveformGeneratorTestFixture, TestWhiteNoise)
{
    auto white_noise = wf_gen.generateWhiteNoise(freq, amplitude, sample_rate);

    auto period_samples = static_cast<size_t>(sample_rate / freq);
    ASSERT_TRUE(period_samples == white_noise.size());

    for (const auto& sample : white_noise) {
        ASSERT_FALSE(sample < -amplitude || sample > amplitude);
    }
}
