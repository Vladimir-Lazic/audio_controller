#ifndef AUDIO_TASK_BUILDER_H
#define AUDIO_TASK_BUILDER_H

enum class WaveformType {
    Sine,
    Sawtooth,
    Triangle,
    Square,
    WhiteNoise,
    None
};

struct AudioTask {
    WaveformType waveform_type { WaveformType::None };
    float frequency { 0 };
    int sample_rate { 0 };
    float amplitude { 0 };
    float phase { 0 };
};

class AudioTaskBuilder {
public:
    static AudioTaskBuilder create() { return AudioTaskBuilder(); }

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

    AudioTaskBuilder& withSampleRate(int sample_rate)
    {
        task.sample_rate = sample_rate;
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
