#ifndef WAVEFORM_GENERATOR_H
#define WAVEFORM_GENERATOR_H

#include <cmath>

class WaveformGenerator {
public:
    WaveformGenerator();
    ~WaveformGenerator();

    void generateSine(float frequency, int duration, int sample_rate);
    void generateSquare(float frequency, int duration, int sample_rate);
    void generateSawtooth(float frequency, int duration, int sample_rate);
    void generateTriangle(float frequency, int duration, int sample_rate);
    void generateWhiteNoise(float frequency, int duration, int sample_rate);
};

#endif // WAVEFORM_GENERATOR_H
