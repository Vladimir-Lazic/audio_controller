#ifndef AUDIO_CONTROLLER_H
#define AUDIO_CONTROLLER_H

#include "Observer.h"
#include "WaveformGenerator.h"
#include <vector>

class AudioController {
public:
    AudioController();
    ~AudioController();

    void play();
    void pause();
    void stop();
    void skip();

    void attachObserver(const Observer& observer);
    void detachObserver(const Observer& observer);
    void notify();

private:
    std::vector<Observer> observers;
    WaveformGenerator wfGen;
};

#endif // AUDIO_CONTROLLER_H
