#ifndef OBSERVER_H
#define OBSERVER_H

#include <optional>
#include <string>

#include "AudioDefinitions.h"

class Subject;

class Observer {
public:
    virtual ~Observer() = default;

    virtual void update(const float& sample) = 0;

    virtual std::optional<AudioTask> listen() = 0;

protected:
    Observer() = default;
};

#endif // OBSERVER_H
