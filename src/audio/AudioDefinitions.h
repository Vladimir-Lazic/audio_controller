#ifndef AUDIO_DEFINITIONS_H
#define AUDIO_DEFINITIONS_H

#include <functional>
#include <unordered_map>
#include <vector>

#include <boost/lockfree/queue.hpp>

enum class WaveformType {
    Sine = 0,
    Sawtooth,
    Triangle,
    Square,
    WhiteNoise
};

enum class PlaybackState {
    Play = 0,
    Stop
};

enum Channels : size_t { Number = 1 };
enum Samples : size_t { Rate = 44100 };
enum Buffer : size_t { Frames = 32 };

struct AudioTask {
    PlaybackState playback_state { PlaybackState::Play };
    WaveformType waveform_type { WaveformType::Sine };
    float frequency { 440.0f };
    float amplitude { 1.0f };
    float phase { 0.0f };
};

using WaveformBuffer = boost::lockfree::queue<float>;
using DispatchMap = std::unordered_map<WaveformType, std::function<float(const AudioTask&, float)>>;

#endif // AUDIO_DEFINITIONS_H
