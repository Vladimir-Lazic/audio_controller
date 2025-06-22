#include "AudioController.h"
#include "RtAudioCallback.h"

AudioController::AudioController(std::shared_ptr<WorkerPool> pool)
    : thread_pool(std::move(pool))
    , playback_buffer(Buffer::Frames)
    , wf_gen(playback_buffer, Buffer::Frames, Samples::Rate)
{
    if (dac.getDeviceCount() < 1) {
        throw std::runtime_error("No audio deviced found!");
    }

    map = {
        { WaveformType::Sine, [this](const AudioTask& req) { return wf_gen.generateSineWave(req.frequency, req.amplitude, req.phase); } },
        { WaveformType::Square, [this](const AudioTask& req) { return wf_gen.generateSquare(req.frequency, req.amplitude, req.phase); } },
        { WaveformType::Sawtooth, [this](const AudioTask& req) { return wf_gen.generateSawtooth(req.frequency, req.amplitude, req.phase); } },
        { WaveformType::Triangle, [this](const AudioTask& req) { return wf_gen.generateTriangle(req.frequency, req.amplitude, req.phase); } },
        { WaveformType::WhiteNoise, [this](const AudioTask& req) { return wf_gen.generateWhiteNoise(req.amplitude); } }
    };

    stream_parameters.deviceId = dac.getDefaultOutputDevice();
    stream_parameters.nChannels = Channels::Number;
    stream_parameters.firstChannel = 0;

    dac.openStream(&stream_parameters,
        nullptr,
        RTAUDIO_FLOAT64,
        Samples::Rate,
        &buffer_frames,
        audioCallback,
        reinterpret_cast<void*>(this));
}

AudioController::~AudioController()
{
    if (dac.isStreamRunning()) {
        dac.stopStream();
    }

    if (dac.isStreamOpen()) {
        dac.closeStream();
    }
}

void AudioController::play(const WaveformBuffer& buffer) const
{
    playback_buffer = buffer;
    dac.startStream();
}

void AudioController::stop() const
{
    if (dac.isStreamRunning()) {
        dac.stopStream();
    }
}

void AudioController::processTask(const AudioTask& task)
{
    if (task.playback_state == PlaybackRequest::Stop) {
        thread_pool->loadTask([this]() {
            stop();
        });
        return;
    }

    auto waveform_buffer = threading::runAsync([this, task = task]() {
        return map.at(task.waveform_type)(task);
    }).share();

    thread_pool->loadTask([this, waveform_buffer]() {
        while (waveform_buffer.wait_for(std::chrono::milliseconds(10)) != std::future_status::ready) { }
        notify(waveform_buffer.get());
    });

    thread_pool->loadTask([this, waveform_buffer]() {
        while (waveform_buffer.wait_for(std::chrono::milliseconds(10)) != std::future_status::ready) { }
        play(waveform_buffer.get());
    });
}

const WaveformBuffer& AudioController::getPlaybackBuffer() const
{
    return playback_buffer;
}
