#ifndef WAVEFORM_GENERATOR_H
#define WAVEFORM_GENERATOR_H

#include <vector>

class WaveformGenerator {
public:
    std::vector<float> generateSineWave(float frequency, float amplitude, int sample_rate, float phase);
    std::vector<float> generateSquare(float frequency, float amplitude, int sample_rate, float phase);
    std::vector<float> generateSawtooth(float frequency, float amplitude, int sample_rate, float phase);
    std::vector<float> generateTriangle(float frequency, float amplitude, int sample_rate, float phase);
    std::vector<float> generateWhiteNoise(float frequency, float amplitude, int sample_rate);
};

#endif // WAVEFORM_GENERATOR_H
