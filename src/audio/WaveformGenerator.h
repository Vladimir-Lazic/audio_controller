#ifndef WAVEFORM_GENERATOR_H
#define WAVEFORM_GENERATOR_H

#include "AudioTaskBuilder.h"
class WaveformGenerator {
public:
    const WaveformBuffer& generateSineWave(float frequency, float amplitude, float phase);
    const WaveformBuffer& generateSquare(float frequency, float amplitude, float phase);
    const WaveformBuffer& generateSawtooth(float frequency, float amplitude, float phase);
    const WaveformBuffer& generateTriangle(float frequency, float amplitude, float phase);
    const WaveformBuffer& generateWhiteNoise(float amplitude);

    explicit WaveformGenerator(WaveformBuffer&, size_t, size_t);
    ~WaveformGenerator() = default;

private:
    size_t buffer_size { 256 };
    size_t sample_rate { 44100 };

    WaveformBuffer& playback_buffer;
};

#endif // WAVEFORM_GENERATOR_H
