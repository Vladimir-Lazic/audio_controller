#ifndef WAVEFORM_GENERATOR_H
#define WAVEFORM_GENERATOR_H

#include "AudioTaskBuilder.h"
class WaveformGenerator {
public:
    WaveformGenerator();
    ~WaveformGenerator() = default;

    const WaveformBuffer& generateSineWave(float frequency, float amplitude, int sample_rate, float phase);
    const WaveformBuffer& generateSquare(float frequency, float amplitude, int sample_rate, float phase);
    const WaveformBuffer& generateSawtooth(float frequency, float amplitude, int sample_rate, float phase);
    const WaveformBuffer& generateTriangle(float frequency, float amplitude, int sample_rate, float phase);
    const WaveformBuffer& generateWhiteNoise(float amplitude);

private:
    WaveformBuffer audio_channel {};
    size_t default_buffer_size { 100 };
};

#endif // WAVEFORM_GENERATOR_H
