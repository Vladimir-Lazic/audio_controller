#ifndef AUDIO_DEFINITIONS_H
#define AUDIO_DEFINITIONS_H

#include <vector>

using WaveformBuffer = std::vector<float>;

enum class WaveformType {
    Sine,
    Sawtooth,
    Triangle,
    Square,
    WhiteNoise
};

struct AudioTask {
    WaveformType waveform_type { WaveformType::Sine };
    float frequency { 440.0f };
    int sample_rate { 441100 };
    float amplitude { 1.0f };
    float phase { 0.0f };
};

enum Channels : size_t { Number = 1 };

#endif // AUDIO_DEFINITIONS_H
