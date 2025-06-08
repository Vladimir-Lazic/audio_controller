#ifndef AUDIO_CONTROLLER_H
#define AUDIO_CONTROLLER_H

#include "Observer.h"
#include "Subject.h"
#include "WaveformGenerator.h"
#include <vector>

class AudioTasks;

class AudioController : public Subject {
public:
    AudioController();
    ~AudioController();

    void play(const std::vector<AudioTasks>& requests);
    void pause(const std::vector<AudioTasks>& requests);
    void stop();

private:
    WaveformGenerator wf_gen;
};

#endif // AUDIO_CONTROLLER_H
