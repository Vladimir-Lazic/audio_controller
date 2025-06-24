#include "AudioController.h"
#include "RtAudioCallback.h"

AudioController::AudioController(std::shared_ptr<WorkerPool> workers, std::shared_ptr<TaskPool> tasks)
    : thread_pool(std::move(workers))
    , task_pool(std::move(tasks))
    , audio_queue(std::make_shared<AudioQueue>(Buffer::Frames))
    , wf_gen(audio_queue)
{
    if (dac.getDeviceCount() < 1) {
        throw std::runtime_error("No audio deviced found!");
    }

    stream_parameters.deviceId = dac.getDefaultOutputDevice();
    stream_parameters.nChannels = Channels::Number;
    stream_parameters.firstChannel = 0;

    dac.openStream(&stream_parameters,
        nullptr,
        RTAUDIO_FLOAT32,
        Samples::Rate,
        &buffer_frames,
        audioCallback,
        reinterpret_cast<void*>(audio_queue.get()));

    dac.startStream();

    alive = true;
    audio_task.playback_state = PlaybackState::Stop;

    thread_pool->loadWorker([this]() {
        while (alive) {
            AudioTask task;
            {
                threading::LockType lock(audio_task_mutex);
                task = audio_task;
            }
            play(task);
        }
    });

    thread_pool->loadWorker([this]() {
        while (alive) {
            float sample = 0.0f;
            if (audio_queue->pop(sample)) {
                notify(sample);
            }
        }
    });
}

AudioController::~AudioController()
{
    alive = false;

    if (dac.isStreamRunning()) {
        dac.stopStream();
    }

    if (dac.isStreamOpen()) {
        dac.closeStream();
    }

    std::cout << "Stream Closed, Audio Controller destroyed" << std::endl;
}

void AudioController::query()
{
    auto task = task_pool->getTask();
    if (task) {
        threading::LockType lock(audio_task_mutex);
        audio_task = *task;
    }
}

void AudioController::play(const AudioTask& task)
{
    if (task.playback_state == PlaybackState::Stop) {
        stop();
        return;
    }

    wf_gen.generate(task);
}

void AudioController::stop()
{
    float dummy;
    while (audio_queue->pop(dummy)) { }
}
