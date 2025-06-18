#ifndef OBSERVER_H
#define OBSERVER_H

#include "AudioDefinitions.h"

#include <optional>

class Subject;

class Observer {
public:
    virtual ~Observer() = default;

    virtual void update(const WaveformBuffer& waveform_buffer) = 0;

    virtual std::optional<AudioTask> listen() = 0;

protected:
    Observer() = default;
};

#endif // OBSERVER_H
