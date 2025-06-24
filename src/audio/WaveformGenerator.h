#ifndef WAVEFORM_GENERATOR_H
#define WAVEFORM_GENERATOR_H

#include "AudioTaskBuilder.h"

#include <memory>

class WaveformGenerator {
public:
    explicit WaveformGenerator(std::shared_ptr<AudioQueue>);
    ~WaveformGenerator() = default;

    void generate(const AudioTask& task);

private:
    void SineWave(float frequency, float amplitude, float phase);
    void Square(float frequency, float amplitude, float phase);
    void Sawtooth(float frequency, float amplitude, float phase);
    void Triangle(float frequency, float amplitude, float phase);
    void WhiteNoise(float amplitude);

    std::shared_ptr<AudioQueue> audio_queue;
    int sample_rate { Samples::Rate };

    DispatchMap map;
};

#endif // WAVEFORM_GENERATOR_H
