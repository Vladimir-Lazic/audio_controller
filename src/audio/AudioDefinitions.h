#ifndef AUDIO_DEFINITIONS_H
#define AUDIO_DEFINITIONS_H

#include <vector>
#include <functional>

using WaveformBuffer = std::vector<float>;

enum class WaveformType {
    Sine = 0,
    Sawtooth,
    Triangle,
    Square,
    WhiteNoise
};

enum class PlaybackRequest {
    Play = 0,
    Stop
};

enum Channels : size_t { Number = 1 };
enum Samples : size_t { Rate = 44100 };
enum Buffer : size_t { Frames = 256 };

struct AudioTask {
    PlaybackRequest playback_state { PlaybackRequest::Play };
    WaveformType waveform_type { WaveformType::Sine };
    float frequency { 440.0f };
    float amplitude { 1.0f };
    float phase { 0.0f };
};

#endif // AUDIO_DEFINITIONS_H
