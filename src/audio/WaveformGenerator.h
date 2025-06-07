#ifndef WAVEFORM_GENERATOR_H
#define WAVEFORM_GENERATOR_H

#include <cmath>
#include <vector>

class WaveformGenerator {
public:
    WaveformGenerator();
    ~WaveformGenerator();

    std::vector<float> generateSine(float frequency, int duration, int sample_rate);
    std::vector<float> generateSquare(float frequency, int duration, int sample_rate);
    std::vector<float> generateSawtooth(float frequency, int duration, int sample_rate);
    std::vector<float> generateTriangle(float frequency, int duration, int sample_rate);
    std::vector<float> generateWhiteNoise(float frequency, int duration, int sample_rate);
};

#endif // WAVEFORM_GENERATOR_H
