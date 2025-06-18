#ifndef OBSERVER_H
#define OBSERVER_H

#include <memory>
#include <optional>
#include <vector>

#include "AudioTaskBuilder.h"

class Subject;

class Observer {
public:
    virtual ~Observer() = default;

    virtual void update(const std::vector<float>& waveform_buffer) = 0;

    virtual std::optional<AudioTask> listen() = 0;

protected:
    Observer() = default;
};

#endif // OBSERVER_H
