#ifndef WAVEFORM_GENERATOR_H
#define WAVEFORM_GENERATOR_H

#include "AudioTaskBuilder.h"
class WaveformGenerator {
public:
    WaveformGenerator() = default;
    ~WaveformGenerator() = default;

    const WaveformBuffer& generateSineWave(float frequency, float amplitude, int sample_rate, float phase);
    const WaveformBuffer& generateSquare(float frequency, float amplitude, int sample_rate, float phase);
    const WaveformBuffer& generateSawtooth(float frequency, float amplitude, int sample_rate, float phase);
    const WaveformBuffer& generateTriangle(float frequency, float amplitude, int sample_rate, float phase);
    const WaveformBuffer& generateWhiteNoise(float frequency, float amplitude, int sample_rate);

private:
    WaveformBuffer audio_channel {};
};

#endif // WAVEFORM_GENERATOR_H
