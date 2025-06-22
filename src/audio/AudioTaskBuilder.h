#ifndef AUDIO_TASK_BUILDER_H
#define AUDIO_TASK_BUILDER_H

#include "AudioDefinitions.h"

class AudioTaskBuilder {
public:
    static AudioTaskBuilder create() { return AudioTaskBuilder(); }

    AudioTaskBuilder& withPlaybackRequest(PlaybackRequest state)
    {
        task.playback_state = state;
        return *this;
    }

    AudioTaskBuilder& withWaveform(WaveformType type)
    {
        task.waveform_type = type;
        return *this;
    }

    AudioTaskBuilder& withFrequency(float freq)
    {
        task.frequency = freq;
        return *this;
    }

    AudioTaskBuilder& withAmplitude(float amplitude)
    {
        task.amplitude = amplitude;
        return *this;
    }

    AudioTaskBuilder& withPhase(float phase)
    {
        task.phase = phase;
        return *this;
    }

    AudioTask build()
    {
        return task;
    }

private:
    AudioTask task;
};

#endif // AUDIO_TASK_BUILDER_H
