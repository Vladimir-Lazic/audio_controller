#ifndef WAVEFORM_GENERATOR_H
#define WAVEFORM_GENERATOR_H

#include <vector>
class WaveformGenerator {
public:
    using Buffer = std::vector<float>;

    WaveformGenerator() = default;
    ~WaveformGenerator() = default;

    const Buffer& generateSineWave(float frequency, float amplitude, int sample_rate, float phase);
    const Buffer& generateSquare(float frequency, float amplitude, int sample_rate, float phase);
    const Buffer& generateSawtooth(float frequency, float amplitude, int sample_rate, float phase);
    const Buffer& generateTriangle(float frequency, float amplitude, int sample_rate, float phase);
    const Buffer& generateWhiteNoise(float frequency, float amplitude, int sample_rate);

private:
    Buffer audio_channel;
};

#endif // WAVEFORM_GENERATOR_H
